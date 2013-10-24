#ifndef QUANTITIES_H
#define QUANTITIES_H

#define NumberOfQuantities 10

class Quantities{

  public:


  void WindingNumber( GraphSpace::Event::Leg *world, GraphSpace::Event::Leg *worldB, int mcs);
  void Density( GraphSpace::Event::Leg *world, int mcs);
  void LocalDensity(GraphSpace::Event::Leg *world);
  void Energy(int countv, int mcs);
  void Susceptibility( double Wlen, int mcs  );
  void NumberOfVertices( int Nv, int Nk );
  void NumberOfKinks( int Nk );
  void DensityDensityCorrelation( GraphSpace::Event::Leg *world, int m_Wnum  );
  void GreenFunction();


  int Nq, Nc;

  void MCsum_S();
  void Bin(int);
  double *values_S;
  double *Bin_S;
  double *MCmean_S;
  double *RNDmean_S;

  void MCsum_L();
  double *values_L;
  double *MCmean_L;
  double *RNDmean_L;

  double *AC;
  double *MCmeanACT;
  double *RNDmeanACT;

  char **file;
  char **acfile;

  int V, Nx, NUM;
  System *sp;
  Size *N;
  Parallel *PR;
  Lattice *LT;
  MC_p *MC;


  void autocorrelation( int mcs, double q ){  AC[mcs]=q; };
  void AutoCorrelationAverage( char *fname );

  double *nvisit;

  private:

  int Npara, Kcut;
  int my_rank;
  double SFD_Norm;
  double *COSrk, *SINrk;

  void SpecificHeat();

  public:


  char parainfo[64];

  Quantities( Size *m_N, MC_p *m_MC, System *m_sp, Lattice *m_LT, Parallel *m_PR  );
  ~Quantities();

  void Init();

  void Output_S(char **fname, double *g); 
  void Output_L(char **fname, double *g); 
  void Output(char fname[128], double x, double g);

  void Measure( int Nv, int Nk, GraphSpace::Event::Leg *world, GraphSpace::Event::Leg *worldB, double length, int m_Wnum, int mcs, int bin);

};


#endif
