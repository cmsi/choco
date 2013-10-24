#ifndef SYSTEMPARAM_H
#define SYSTEMPARAM_H

#define VAL 10
#define INF 1.0e+14
#define EPS 1.0e-14

struct Size{

  int x;
  int y;
  int z;
  int V;
  int d;
  double B;

};


struct System{

  int nmax;
  double Vc;
  double tb;
  double mu;

};

struct MC_p{

  int seed;
  int Nstep;
  int Nthermal;
  int Nsample;
  int Nchain;
  int nc;

};

struct Parallel{

  int p_num;
  int my_rank;
  int Npara;
  int np;

};

#endif
