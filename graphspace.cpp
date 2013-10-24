#include <Configuration.h>
#include <algorithm>
//#define debug
//この中のBはすべて分割空間のB
//#include <fj_tool/fapp.h>


GraphSpace::GraphSpace(Size *N,int m_nmax, Parallel *m_PR,  long long  m_IMAX, My_rdm *MR, System *m_sp){

  IMAX = m_IMAX;
  NMIN = G_NMIN;
  PR = m_PR;
  sp=m_sp;

  V = N->V;
  B = N->B;
  D = N->d;
  Nx=N->x;
  PI = M_PI;
  Ckin = 4.0*PI*sp->tb;

  Vcenter = N->x/2.0;

  nmax = m_nmax;
  Bh=B/2.0;

  my_rank = PR->my_rank;
 
  initialev();

}

GraphSpace::~GraphSpace(){

  delcall( nmax );
  delcall( NW_before );
  delcall( vertex );
  delcall(ver_time);

}

void GraphSpace::initialize(){

  newcalls( w, nmax );
  newcalls( NW_before, nmax );
  newcalls( vertex, 2 );
  newcall(ver_time,IMAX);

  ev.resize(IMAX);

  L_IMAX=IMAX/nmax;
  world.resize(nmax);

  if((int)world.size()!=nmax ) cout<<"error of world!! "<<endl;
  for( int i=0; i <nmax ; i++ ) world[i].resize(L_IMAX);

 
  C_vc = sp->Vc*Nx*Nx/4.0; //定数シフト
  vc_tot = sp->Vc*Nx*Nx*Nx/12.0; //積分値

  for(int i=0; i<nmax; i++){

    world[i][0].next[1] = &(world[i][0]);
    world[i][0].next[0] = &(world[i][0]);
    world[i][0].t = 0.0;
    world[i][0].type = -1;
  }

}


void GraphSpace::Diagonal_Update( My_rdm *MR ){


  Assign_Vertex( MR );
  Remove_Vertex( MR );

}

inline double GraphSpace::poteitial(double r){ return sp->Vc*( Nx - r )*r ); } //sp->Vc*( (Nx/2)**2 - (r - Nx/2)**2 )

//#############################################################################################
void GraphSpace::Assign_Vertex( My_rdm *MR ){

  double R, P;
  double lw;
  double tau=0.0;

  double x;
  int d=0, num;

  for(int i=0; i<nmax; i++){

    w[i] = &(world[i][0]);
  }

  for( int i=0; i < nmax; i++ )  NW_before[i] = world[i].size();

  while( 1 ){


    lw = -log(1.0-R)/vc_tot; //虚時間方向の進む長さ
    tau += lw;
    if( tau > B ) break;

    x = Nx*MR->rdm(); //位置、要検討?
    num = (int)(nmax*MR->rdm()); //どの粒子か決める
    if( world[num].size() > L_IMAX ) continue;

    while( w[num]->next[1] < targ_time && w[num]->next[1]->t != 0.0 ) w[num] = w[num]->next[1];

    R = MR->rdm();
    P = potential(x)/vc_tot; //本当に入れるか決める

    if( R < P ){ 

      if( world[num].[0] == world[num].[0]->next[1] || kinetic(num,tau,x){ connect_Vertex( 1, tau, x, num, d );  Nv++; }

  }   


}

bool GraphSpace::kinetic( int num, double t_new, double x_new ){

  double t_belower=w[num]->t, t_upper=w[num]->next[1]->t;
  double R = MR->rdm();
  double P;
  double dim = -0.5; //-d/2

  if( world[num].[0] == world[num].[0]->next[1]->next[1]  ){

    double tau1, tau2, r1;
   if(t_upper==0.0){ tau1 = t_new - t_belower ; r1 = w[num]->x - x_new; }
   else{     tau1 = t_upper - t_new;  r1 = w[num]->next[1]->x - x_new;  }
 
   double tau2=(B-tau1)*(tau1)/B;

    P = pow(Ckin*tau2,dim)*exp(r1*r1/(4.0*sp->tb*tau2));

  }
  else{



  }


  return ( R < P );


}


void GraphSpace::connect_before(  Vertex *wl, Vertex *new_vertex ){

  new_vertex->next[1] = wl->next[1];
  new_vertex->next[0] = wl;        
	  
}

void GraphSpace::connect_after( Vertex **wl, Vertex *wnew, int num ){
  
  wl[num]->next[1]->next[0] = wnew;  
  wl[num]->next[1] = wnew;

  wl[num] = wnew;
  
}


void GraphSpace::connect_Vertex( short TYPE, double targ_time, double x, int num, int d ){

  Vertex new_vertex;

  new_vertex.type = TYPE; 

  new_vertex.t = targ_time;
  new_vertex.x = x+d*Nx; 

  connect_before( w[num], &(new_vertex) );

  world[num].push_back( new_vertex );
      
  connect_after( w, &(world[num].back()), num );


}

void GraphSpace::Remove_Vertex( MR ){


}

