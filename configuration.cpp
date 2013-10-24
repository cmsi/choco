#include <Configuration.h>

//************************************Configurationクラスここから***********************************************************
//#define Bcheck


Configuration::Configuration( MC_p *m_MC,Size *m_N, int m_nmax,Parallel *PR, long long m_IMAX, My_rdm *MR, System *m_sp):GraphSpace(m_N,m_nmax,PR,m_IMAX,MR,m_sp){

  N=m_N;
  p_num=PR->p_num;
  MC=m_MC;

}

Configuration::~Configuration(){

}


void Configuration::update( int MCS, My_rdm *MR, Quantities *QNT ){


    while(MCS--) Diagonal_Update(MR));

}

void Configuration::update( Quantities *QNT, My_rdm *MR ){//測定用

  QNT->Init();

  for(int j=0; j<MC->Nstep; j++){

    Diagonal_Update(MR);

    for(int i=0; i<MC->Nsample; i++){

      QNT->Measure( Nv, Nk, world, worldB, Wlen, Wnum[0], i, j);//配置//Wnum.sizeは変わらないけど、Wnumは変わっている
    }
  }

  QNT->MCsum_S();


}

//************************************Configurationクラス終わり***********************************************************

