//#include<stdma.h>
#include<Configuration.h>
//ここでのpnumはNparaのこと
//#define ATtime
//#include <fftw3.h>

Quantities::Quantities( Size *m_N, MC_p *m_MC, System *m_sp, Parallel *m_PR ){

  Npara=m_PR->Npara;
  my_rank=m_PR->my_rank;

  MC=m_MC;
  N=m_N;
  PR=m_PR;

  sp=m_sp;
  V=N->V;
  Nx=N->x;

  SFD_Norm=N->B*2.0*sp->tb/pow((double)V,(N->d-2.0));
  sprintf(parainfo,"B%.1lf_Nx%d_Ny%d_Vbb%.1lf",N->B,N->x,N->y,sp->Vb1);

  Nc=3;
  Nq=7;

  Kcut=(V*0.5 > 100)? 100: V*0.5;

  newcall(file,Nc+Nq,128);


   sprintf(file[0],"WN_%s.dat",parainfo);
   sprintf(file[1],"Dns_%s.dat",parainfo);
   sprintf(file[2],"Eng_%s.dat",parainfo);
   sprintf(file[3],"SUS_%s.dat",parainfo);
   sprintf(file[4],"SH_%s.dat",parainfo);
   sprintf(file[5],"Nv_%s.dat",parainfo);
   sprintf(file[6],"Nk_%s.dat",parainfo);
  
   sprintf(file[7],"LD_%s.dat",parainfo);
   sprintf(file[8],"Sk_%s.dat",parainfo);
   sprintf(file[9],"GF_%s.dat",parainfo);



  newcall(values_S,Nq);
  newcall(Bin_S,Nq*MC->Nstep);
  newcall(MCmean_S,Nq*2);
  newcall(RNDmean_S,Nq*2*PR->Npara);

  newcall(values_L,V*Nc);
  newcall(MCmean_L,V*Nc);
  newcall(RNDmean_L,V*Nc*PR->Npara);

  newcall(nvisit,V);
  newcalls(COSrk,Kcut*V);
  newcalls(SINrk,Kcut*V);

  double PI=M_PI;
  for(int i=0;i<V;i++){
    int x=i%Nx;    int y=i/Nx;

    for(int k=0;k<Kcut;k++){
      double phase= x*(Nx-k)*PI/Nx + y*PI;
      COSrk[i+k*V]=cos(phase);
      SINrk[i+k*V]=sin(phase);
    }

  }

#ifdef ATtime

  newcall(acfile,Nq,128);

  sprintf(acfile,fname);

  newcall(AC,MC->Nsample);
  newcall(MCmeanACT,2);
  newcall(RNDmeanACT,2*PR->Npara);
#endif

  
}

Quantities::~Quantities(){

  delcall(file,Nq+Nc);

  delcall(values_S);  
  delcall(Bin_S);
  delcall(MCmean_S);
  delcall(RNDmean_S);

  delcall(values_L);  
  delcall(MCmean_L);
  delcall(RNDmean_L);

  delcall(nvisit);
  delcall(COSrk);
  delcall(SINrk);

#ifdef ATtime
  delcall(acfile,Nq);
  delcall(AC);
  delcall(MCmeanACT);
  delcall(RNDmeanACT);
#endif


}

void Quantities::Measure( int Nv, int Nk, GraphSpace::Vertex *world, GraphSpace::Vertex *worldB, double length, int m_Wnum, int mcs, int bin ){




    WindingNumber(world,worldB,mcs);
    Density(world,mcs);
    Energy(Nv, mcs);
    Susceptibility(length,mcs);
    NumberOfVertices(Nv,Nk);
    NumberOfKinks(Nk);
 

    Bin(bin);

    DensityDensityCorrelation(world,m_Wnum);

    MCsum_L();


}


void Quantities::Init(){

  for( int i=0; i<Nq; i++ ) values_S[i]=0;
  for( int i=0; i<Nc*V; i++ ) values_L[i]=0;
  for( int i=0; i<V; i++ ) nvisit[i]=0;
  for(int i=0; i<Nq*MC->Nstep; i++) Bin_S[i] = 0;


}

void Quantities::Output_S(char **fname, double *g){//通常

  FILE *file, *files;
  char efname[128],source[128];//***
  int Spara = PR->Spara, Mpara = PR->Mpara;
  int S=Nq*2;


  for(int k=0; k<S; k+=2){

    file = fopen(fname[k/2],"a+");


    sprintf(source,"source_%s",fname[k/2]);//***
    files = fopen(source,"a+");//***

    for(int j=0; j<Mpara; j++){

      double mean=0.0, error=0.0, sigma=0.0, s_error=0.0;
      double MU = sp->mu + j*sp->dmu;

      for(int i=0; i<Spara; i++){

	int p=j+i*Mpara;

	mean += g[k + p*S];
	error += g[k + p*S]*g[k + p*S];
	sigma += g[k+1 + p*S];
	s_error += g[k+1 + p*S]*g[k+1 + p*S];



	fprintf(files,"%lf %lf %lf %d %le %le\n", MU, sp->Vb1, N->B, N->x, g[k + p*S], g[k+1 + p*S] );//***
      }
    

      mean = mean/(double)Spara;
      error = error/(double)Spara;
      sigma = sigma/(double)Spara;
      s_error = s_error/(double)Spara;

      if(Spara==1){
	error = 0.0;
	s_error = 0.0;
      }
      else{
	error = sqrt( ( error - mean*mean )/(Spara-1.0) );
	s_error = sqrt( ( s_error - sigma*sigma )/(Spara-1.0) );
      }

      fprintf(file,"%lf %lf %lf %d %le %le %le %le\n", MU, sp->Vb1, N->B, N->x, mean, error, sigma, s_error );

    }

    fclose(file);
    fclose(files);//***

  }


}

void Quantities::Output_L(char **fname, double *g){//通常

  FILE *file;

  int Spara=PR->Spara, Mpara=PR->Mpara;
  double num =(Spara==1)? 1.0:  Spara-1.0;
  int S=Nc*V;
  int k, L;
  double R;
  char xxfname[256];

  for(int j=0; j<Nc; j++){


    for(int m=0; m<Mpara; m++){

      double MU=sp->mu + m*sp->dmu;

      sprintf(xxfname,"X%lf_%s",MU,fname[j+Nq]);

      file = fopen(xxfname,"a+");

      L=(j==1)? Kcut: V;

      for(int l=0; l<L; l++){
	
	k=l+j*V;

	double mean=0.0, error=0.0;

	for(int i=0; i<Spara; i++){

	  int p=m+i*PR->Mpara;
	  mean += g[k + p*S]/(double)MC->Nsample;
	  error += (g[k + p*S]/(double)MC->Nsample)*(g[k + p*S]/(double)MC->Nsample);
	  
	}

	mean = mean/Spara;
	error = error/Spara;
	error = sqrt( ( error - mean*mean )/num );
	
	if(j<2) R=l;
	else { 
           if(l<Nx){ R = l;}
           else continue;
        }
	fprintf(file,"%lf %lf %lf %d %lf %le %le\n", MU, sp->Vb1, N->B, N->x, R, mean, error );
      }
      

      fclose(file);
    }

  }


}

void Quantities::Output(char fname[128], double x, double g){//エラーバーなし

  FILE *file;
  file = fopen(fname,"a+");

  fprintf(file,"%lf %le\n", x, g );

  fclose(file);

}

void Quantities::MCsum_S(){


  SpecificHeat();

  double num =(MC->Nstep==1)? 1.0:  MC->Nstep-1.0;

  for(int i=0; i<Nq; i++){

    double mean=0.0, s_mean=0.0;

    for(int j=0; j<MC->Nstep; j++){

      double val=Bin_S[i+j*Nq];
      mean += val;
      s_mean += val*val;
    }

    mean/=(double)MC->Nstep;
    s_mean/=(double)MC->Nstep;

    s_mean = sqrt((s_mean -mean*mean)/num);

    MCmean_S[2*i] = mean;
    MCmean_S[2*i+1] = s_mean;


  }


}


void Quantities::Bin(int bin){

  for(int i=0; i<Nq; i++){
      Bin_S[i+bin*Nq] += values_S[i]/(double)MC->Nsample;
  }

}


void Quantities::MCsum_L(){

  int size=100*Nx;

  for(int i=0; i<size; i++){
      MCmean_L[i] += values_L[i];
  }

}

//*******************ワインディングナンバー************************

void Quantities::WindingNumber(GraphSpace::Vertex *world, GraphSpace::Vertex *worldB, int mcs){


}

//*******************密度************************

void Quantities::Density(GraphSpace::Vertex *world, int mcs){


}

//*******************エネルギー************************

void Quantities::Energy( int countv, int mcs ){


}


//*******************感受率************************

void Quantities::Susceptibility( double Wlen, int mcs  ){



}

//*******************局所密度************************

void Quantities::LocalDensity(GraphSpace::Vertex *world){

  double size=100*Nx;
  for(int i=0; i<size; i++) { values_L[i] ==0; }

  for( int i=0; i<nmax; i++ ){
    for( int j=0; j<(int)world[i].size(); j++ ){

      int site = world[i][j].i*100;
       n[site] ++;
   }
 }

  


}

//*******************vertex数************************

void Quantities::NumberOfVertices( int Nv, int Nk  ){



}

//*******************kink数************************

void Quantities::NumberOfKinks( int Nk  ){



}

//*********
void Quantities::SpecificHeat(){



  
}

//*******************密度相関************************

void Quantities::DensityDensityCorrelation( GraphSpace::Vertex *world, int m_Wnum ){



}

//*******************グリーン関数************************

void Quantities::GreenFunction(){


}


//##########################################################################################


