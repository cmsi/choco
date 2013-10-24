#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//#define triangle
#define actime

#include <stdio.h>

#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
using namespace std;

#include <stack>
#include <vector>
#include <list>
#include<algorithm>
#include <stdma.h>
#include <systemparameter.h>
#include <My_rdm.boost.h>
//#include <My_rdm.gsl.h>
//#include <Probability.h>
#include <mpi.h>

#define G_NMIN 1.0e-8
#define NSAMPLE 100000


//#include <Lattice.h>

//************************************GraphSpaceクラスここから***********************************************************

class Quantities;

class GraphSpace{

  public:

  double NMIN;

  int my_rank;

  class Vertex{

      public:
      double t; //時間
      double x;//サイト
      short type;//イベントのタイプ・・・ 偶数：右、奇数：左、
                   //  -1(tau=0),-2(tau=B):虚時間境界edge, 1:potential, 2:mu, 3:interparticle-interaction

      Vertex *next[2]; //上と下
      Vertex *nleg; //となり(interactionのときにつかう)

  };


  Parallel *PR;
 System *sp;

  std::vector<Vertex> world;


  double vc_tot;
  int Nv;

  double B, Bh, Vcenter;
  int V, Nx, D, nmax;
  long long IMAX;
  int L_IMAX;

  double Ckin, PI;

  GraphSpace(Size *N,int m_nmax, Parallel *m_PR,  long long  m_IMAX, My_rdm *MR, System *m_sp);
  ~GraphSpace();


  protected:

  void Diagonal_Update( My_rdm *MR );

  private:

  Vertex **w;
  Vertex **vertex;
  Vertex *NW_before;


  double *ver_time;

  //vertex
  void initialize( My_rdm *MR);
  void move_vertex( short type, int k );

  void Assign_Vertex( My_rdm *MR );

  void connect_before( Vertex *wl, Vertex *new_event );
  void connect_after( Vertex **wl, Vertex *el, int x );

  void connect_Vertex(  short TYPE, double targ_time, double x, int num, int d );
  inline double poteitial(double r);
  bool kinetic( int num, double t_new, double x_new );
  void Remove_Vertex( MR );

};

//************************************GraphSpaceクラスおわり***********************************************************

//************************************Configurationクラスここから***********************************************************

#include <Quantities.h>

class Configuration : public GraphSpace {

  Size *N;
  void initialization();
  double BV;
  int p_num;
  MC_p *MC;
  public:

  Configuration( MC_p *m_MC, Size *N, int m_nmax, Parallel *PR,  long long  m_IMAX, My_rdm *MR, System *m_sp );
  ~Configuration();

  void update( int MCS, My_rdm *MR, Quantities *QNT);
  void update( Quantities *QNT, My_rdm *MR );

};

//************************************Configurationクラスおわり***********************************************************


#endif

