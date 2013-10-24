#include <My_rdm.h>

My_rdm::My_rdm(long long seed){

  if( seed == 0 )  gsl_rng_env_setup(); 
  else	gsl_rng_default_seed=(unsigned long)seed;

  type_gsl_rng = gsl_rng_default; 
  generator_gsl_rng = gsl_rng_alloc(type_gsl_rng); 

}

My_rdm::~My_rdm(){

	gsl_rng_free(generator_gsl_rng);
}

long double My_rdm::gsl_rng_rdm_ld(){

	return (long double)gsl_rng_uniform(generator_gsl_rng);
}
double My_rdm::gsl_rng_rdm_d(){

	return (double)gsl_rng_uniform(generator_gsl_rng);
}
double My_rdm::rdm(){

	return gsl_rng_rdm_d();
}
long double My_rdm::rdml(){

	return gsl_rng_rdm_ld();
}

int My_rdm::r01(){

  return (int)gsl_rng_rdm_d()*2;
}

int My_rdm::r012(){

  return (int)gsl_rng_rdm_d()*3;
}

