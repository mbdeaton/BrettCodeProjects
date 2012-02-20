// Integrating the neutrino-antineutrino annihilation energy deposition at a point
//   in space given the distribution functions f_nu(theta,phi,E), f_nubar(theta,phi,E).
//   Actually this is merely a test of integrating an analytic function in 6 dimensions.
//   Here I assume the nu and nubar distribution functions are equal, and piecewise constant:
//   fnu(theta,phi,E) = { 1    (theta > 3pi/4) && (E < 10MeV)
//                      { 0    otherwise

#include<cstdlib>
#include<ctime>
#include<iostream>
#include<iomanip>
#include<cmath>
#include<string>
#include<gsl/gsl_math.h>
#include<gsl/gsl_monte.h>
#include<gsl/gsl_monte_plain.h>
#include<gsl/gsl_monte_vegas.h>
#include<gsl/gsl_monte_miser.h>
#include<gsl/gsl_rng.h>

// global vars
const double pi = M_PI;
const double sin2theta_w = 0.23; // weinberg angle sine squared
const double G_F2 = 2.06e-32; // Fermi constant squared (cm^2/erg^2)
const double K = 0.124311; // dimensionless parameter from Harikae 2010
const double c = 2.998e10; // speed of light (cm/s)
const double h = 6.626e-27; // planck's constant (erg s)
const double eVToErgFactor = 1.602e-12;
const double Ebreak = 1e7*eVToErgFactor; // max energy of neutrinos (erg)
const double Escale = Ebreak; // substitute E->E/Escale in integrand to normalize and then
//                               multiply integral by Escale^9 at the end
const double theta_half_opening_angle = pi/4.;
const double HarikaeConst = 2*K*G_F2/pow(c,5)/pow(h,6); // integral factor, multiply result by this
const double result_exact_scaled = 0.1583; // from Mathematica Analytic integration, scaled so that
//                                            this*HarikaeConst*(Escale^9) is in erg/cm^3/s

// Neutrino distribution function (h^{-3} factored out, but not Escale)
// fnu(theta,phi,E)
//   theta    polar angle of incoming neutrino ray
//   phi      azimuthal angle of incoming neutrino ray
//   E        neutrino energy in erg, not normalized, that's done in f
double fnu(const double& theta, const double& phi, const double& E) {
  if ((theta>(pi-theta_half_opening_angle)) && (E<Ebreak))
    return 1;
  else
    return 0;
} // END Inline fnu

// AntiNeutrino distribution function (h^{-3} factored out, but not Escale)
// fnubar(thetabar,phibar,Ebar)
//   thetabar    polar angle of incoming antineutrino ray
//   phibar      azimuthal angle of incoming antineutrino ray
//   Ebar        antineutrino energy in erg, not normalized, that's done in f
double fnubar(const double& thetabar, const double& phibar, const double& Ebar) {
  return fnu(thetabar,phibar,Ebar); // in this test implementation fnubar==fnu;
} // END Inline fnubar

// Total Integrand (divided by HarikaeConst, and scaled by Escale^7; the other two factors of
//   Escale come from dE and dEbar).
// f = fnu(theta,phi,Escaled*Escale) * fnubar(thetabar,phibar,Ebarscaled*Escale) * otherstuff
//   x       array of size dim (==6), giving coordinates {theta,thetabar,phi,phibar,E,Ebar}
//              x[0]=theta       polar angle of incoming neutrino ray
//              x[1]=thetabar    polar angle of incoming antineutrino ray
//              x[2]=phi         azimuthal angle of incoming neutrino ray
//              x[3]=phibar      azimuthal angle of incoming antineutrino ray
//              x[4]=Escaled     neutrino energy
//              x[5]=Ebarscaled  antineutrino energy
//   dim     number of dimensions in x
//   params  pointer to parameters for the function
double f(double* x, size_t dim, void* params) {
  if (dim != 6) {
    std::cout << "Error, dimension argument is not correct" << std::endl;
    return GSL_NAN; // error
  }
  const double trigterm = 1. - sin(x[0])*sin(x[1])*cos(x[2]-x[3]) - cos(x[0])*cos(x[1]);
  const double Eterm = (x[4]+x[5])*pow(x[4],3)*pow(x[5],3);
  const double fterm = fnu(x[0],x[2],x[4]*Escale)*fnubar(x[1],x[3],x[5]*Escale);
  return fterm*trigterm*trigterm*Eterm;
} // END Inline f

// Print the result of a particular calculation
//   method       monte carlo technique used to obtain result
//   N            number of sampled points
//   result       value of the integral, normalized by ConstHarikae and Escale^9
//   sigma        error approximation to result, also normalized
//   wallseconds  elapsed walltime for calculation
void print_result(const std::string method, const int N,
		  const double result, const double sigma,
		  const unsigned int wallseconds) {
  std::cout << method << " (N=" << N << ") =======================" << std::endl;
  std::cout << "  time   = " << wallseconds << " sec" << std::endl;
  std::cout << std::setprecision(16);
  std::cout << "  result = " << result << std::endl
	    << "  exact  = " << result_exact_scaled << std::endl
	    << "  sigma  = " << sigma << std::endl;
  std::cout << std::setprecision(3);
  std::cout << "  error  = " << fabs(result-result_exact_scaled)/sigma << " sigma" << std::endl;
} // END Inline print_result

void help(const std::string exec_name, const int num_expected_args) {
  std::cout << "Usage error. This executable requires " << num_expected_args
            << " arguments at the command line. Thus:" << std::endl
            << exec_name << " N" << std::endl
            << "  N     int, number of function calls for integrations" << std::endl
            << "example: "<< exec_name << " 10000" << std::endl;
} // END Inline help

int main (int argc, char** argv) {
  const int num_expected_args(1);
  if (argc!=num_expected_args+1) {
    help(argv[0],num_expected_args);
    return 1;
  }

  // DEBUG: sanity checks
  std::cout << "Ebreak (erg) = " << Ebreak << std::endl;
  std::cout << "Escale (erg) = " << Escale << std::endl;
  std::cout << "theta_half_opening_angle = " << theta_half_opening_angle << std::endl;
  std::cout << "HarikaeConst = " << HarikaeConst << std::endl;
  double xcheck[6] = {pi,0.9*pi,0,0,0.9*Ebreak/Escale,0.9*Ebreak/Escale};
  std::cout << "integrand(pi,0.9pi,0,0,9MeV/Escale,9MeV/Escale) = "
	    << f(xcheck,6,NULL) << " (should be 0.00229149)" << std::endl;

  gsl_rng* rng = gsl_rng_alloc(gsl_rng_ranlxd2);
  unsigned int seed(time(NULL));
  gsl_rng_set(rng,seed);
  const size_t dim(6);
  // integration limits
  double x_low[dim]  = {0,0,0,0,0,0};
  double x_high[dim] = {pi,pi,2.*pi,2.*pi,1.2,1.2};
  gsl_monte_function F = {&f, dim, NULL}; // convert the c++ function to type gsl_monte_function
  size_t N_calls = atoi(argv[1]); // number of function calls
  double result; // monte carlo estimate of the integral
  double sigma; // monte carlo estimate of the 1 stdev error

  unsigned int start_time; // seconds
  unsigned int elapsed_walltime;
  {
    std::string method("Plain");
    start_time = time(NULL);
    gsl_monte_plain_state *s = gsl_monte_plain_alloc(dim);
    gsl_monte_plain_integrate(&F, x_low, x_high, dim, N_calls, rng, s,
			      &result, &sigma);
    elapsed_walltime = time(NULL) - start_time;
    gsl_monte_plain_free(s); // free up the memory
    print_result(method, N_calls,
		 result, sigma,
		 elapsed_walltime);
    // print correctly scaled result
    std::cout << "  Q = " << result*HarikaeConst*pow(Escale,9)
	      << " +- " << sigma*HarikaeConst*pow(Escale,9) << std::endl;
  }
  

  {
    std::string method("Miser");
    start_time = time(NULL);
    gsl_monte_miser_state *s = gsl_monte_miser_alloc(dim);
    gsl_monte_miser_integrate(&F, x_low, x_high, dim, N_calls, rng, s,
			      &result, &sigma);
    elapsed_walltime = time(NULL) - start_time;
    gsl_monte_miser_free(s); // free up the memory
    print_result(method, N_calls,
		 result, sigma,
		 elapsed_walltime);
    // print correctly scaled result
    std::cout << "  Q = " << result*HarikaeConst*pow(Escale,9)
	      << " +- " << sigma*HarikaeConst*pow(Escale,9) << std::endl;
  }

  {
    std::string method("Vegas");
    start_time = time(NULL);
    gsl_monte_vegas_state *s = gsl_monte_vegas_alloc(dim);
    gsl_monte_vegas_integrate(&F, x_low, x_high, dim, N_calls, rng, s,
			      &result, &sigma);
    elapsed_walltime = time(NULL) - start_time;
    const double chisq = gsl_monte_vegas_chisq(s); // Should be ~1. Otherwise values from different
    //                                                iterations are statistically inconsistent.
    gsl_monte_vegas_free(s); // free up the memory
    print_result(method, N_calls,
		 result, sigma,
		 elapsed_walltime);
    std::cout << "  chisq  = " << chisq << std::endl;
    // print correctly scaled result
    std::cout << "  Q = " << result*HarikaeConst*pow(Escale,9)
	      << " +- " << sigma*HarikaeConst*pow(Escale,9) << std::endl;
  }
  
  gsl_rng_free(rng); // free up the memory
  return 0;
}
