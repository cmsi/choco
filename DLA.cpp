#include <DLA.h>
#include <time.h>

#define check
//#define Assessment


int main(int argc, char **argv){

  Parallel PR;

  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &PR.my_rank);
  MPI_Comm_size (MPI_COMM_WORLD, &PR.p_num);

  Size N;
  System sp;
  MC_p MC;

  long long IMAX, WMAX;
  Dla Sim;

  Sim.ReadBuffer(&MC, &N, &sp, &PR, IMAX);
   ////////////////////////////////////////////////////////////
   Sim.DLA_core( &MC, &N, &sp, &PR, IMAX );
  ////////////////////////////////////////////////////////////


  MPI_Finalize ();

  return 0;

}

//*************************************************************************************************

//#define Wcheck

double Dla::DLA_core( MC_p *MC, Size *N, System *sp, Parallel *PR, long long IMAX ){

  //##########################################################################
  #ifdef check
  std::cout <<"rank="<<PR->my_rank <<", "<< MC->Nstep <<' '<< MC->Nthermal <<"  B="<< N->B 
           <<"  IMAX="<< IMAX <<"  nmax="<< sp->nmax << "  Nd=" << N->d << "  Npara=" << PR->Npara << std::endl;
  #endif

  int start, end;
  double XX=sp->mu;

  //##########################################################################

  My_rdm MR(MC->seed);
  My_rdmi MRi(MC->seed+100000,N->V);
  Configuration CS(MC,N,sp->nmax,PR,IMAX,&MR);
  Quantities QNT( N, MC, sp, PR ); 

  //##########################################################################
  
  ////////////////// Thermarization ////////////////////////

    /////////////////////////////////////////////////////////
    MPI_Barrier(MPI_COMM_WORLD); 
    start = MPI_Wtime(); 
    /////////////////////////////////////////////////////////

    CS.update(MC->Nthermal, &MR, &QNT);

    /////////////////////////////////////////////////////////
    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();
    if(PR->my_rank==0) count_time(end-start,XX,"ThermalTime.dat");
    /////////////////////////////////////////////////////////


  /////////////////////////////////////////////////////////
  ////////////////// Observation ////////////////////////
  /////////////////////////////////////////////////////////
  MPI_Barrier(MPI_COMM_WORLD); 
  start = MPI_Wtime(); 
  /////////////////////////////////////////////////////////

  CS.update( &QNT, &MR );

  /////////////////////////////////////////////////////////
  MPI_Barrier(MPI_COMM_WORLD);
  end = MPI_Wtime();
  if(PR->my_rank==0) count_time(end-start,XX,"ObsTime.dat");
  /////////////////////////////////////////////////////////

  ////////////////// Average ////////////////////////
  RNDsum(QNT.MCmean_S, QNT.RNDmean_S, PR->my_rank, QNT.Nq*2,    PR->Npara,PR->np);
  RNDsum(QNT.MCmean_L, QNT.RNDmean_L, PR->my_rank, QNT.Nc*N->V, PR->Npara,PR->np);


  if( PR->my_rank==0 ){//並列化しても変わらない

   QNT.Output_S(QNT.file, QNT.RNDmean_S);
   QNT.Output_L(QNT.file, QNT.RNDmean_L);

   ////////////////// Output the Configuration ////////////////////////

  }
 
  else return 0;

  //////////////////////////////////////////////////////////////////////

}

void  Dla::RNDsum(double *MCmean, double *RNDmean, int my_rank, int Lmax, int Npara, int np){

  MPI_Status status;

  if (my_rank==0){//gather over para

      for(int i=0; i< Lmax; i++ ) RNDmean[i]=MCmean[i];

      for(int tag=1; tag< Npara; tag++ ){

        MPI_Recv(&(RNDmean[tag*Lmax]), Lmax, MPI_DOUBLE, tag, 0, MPI_COMM_WORLD, &status);

     }
  }
  else{
      MPI_Send(MCmean, Lmax, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD );
  }


}


void Dla::ReadBuffer(MC_p *MC, Size *N, System *sp, Parallel *PR, long long &IMAX){


  if (PR->my_rank==0){ 
 
    std::cin >> MC->Nstep >> MC->Nthermal >> N->B >> N->x >> N->y >> N->z >> IMAX >> sp->tb >> sp->mu >> MC->seed >> sp->Vc >> sp->nmax >> N->d >> PR->Spara;
  }


  MPI_Bcast(&MC->Nstep, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&MC->Nthermal, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&N->B, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&N->x, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&N->y, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&N->z, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&IMAX, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

  MPI_Bcast(&sp->tb, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );
  MPI_Bcast(&sp->mu, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );
  MPI_Bcast(&MC->seed, 1, MPI_INT, 0, MPI_COMM_WORLD );
  MPI_Bcast(&sp->Vc, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );
  MPI_Bcast(&sp->nmax, 1, MPI_INT, 0, MPI_COMM_WORLD ); 
  MPI_Bcast(&N->d, 1, MPI_INT, 0, MPI_COMM_WORLD ); 


  MC->seed += PR->my_rank;

  PR->np = PR->my_rank;

  MC->Nsample = MC->Nthermal;
  N->V = N->x*N->y*N->z;


  PR->Npara=PR->p_num;//乱数並列数


}

void Dla::W_check(){

  
}



Dla::Dla(){

}

Dla::~Dla(){

 
}

