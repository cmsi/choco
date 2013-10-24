#ifndef DLA_H
#define DLA_H

#include <Configuration.h>

//////////////////////////////////////////////////////////////////////////////////////////

class Dla{


  public:

  void ReadBuffer(MC_p *MC, Size *N, System *sp, Parallel *PR, long long &IMAX);
  void DLA_core( MC_p *MC, Size *N, System *sp, Parallel *PR, long long IMAX );
  void RNDsum(double *local, double *global, int my_rank, int rmax2, int Npara, int);

  Dla();
  ~Dla();

  private:

  void W_check();

};


#endif
