// Integrating the neutrino-antineutrino annihilation energy deposition at a point
//   in space given the distribution functions f_nu(theta,phi,E), f_nubar(theta,phi,E).
//   Actually this is merely a test of integrating an analytic function in 6 dimensions.
//   Here I assume the nu and nubar distribution functions are equal, and piecewise constant:
//   fnu(theta,phi,E) = { 1    (theta > 3pi/4) && (E < 10)
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
// TODO: put all constants into same system of units
const double pi = M_PI;
const double sin2theta_w = 0.23; // weinberg angle sine squared
const double G_F2 = 2.06e-32; // Fermi constant squared (cm^2/erg^2)
const double K = 1.23; // dimensionless parameter from Harikae 2010
const double c = 3.00e10; // speed of light (cm/s)
const double scale = 2*K*G_F2/pow(c,5); // scale factor, multiply result by this
const double result_exact = 1.583e8*scale; // from Mathematica Analytic integration

// Neutrino distribution function
// fnu(theta,phi,E)
//   theta  polar angle of incoming neutrino ray
//   phi    azimuthal angle of incoming neutrino ray
//   E      neutrino energy
double fnu(const double& theta, const double& phi, const double& E) {
  if ((theta>(3.*pi/4.)) && (E<10.))
    return 1;
  else
    return 0;
} // END Inline fnu

// AntiNeutrino distribution function
// fnubar(theta,phi,E)
//   thetabar  polar angle of incoming antineutrino ray
//   phibar    azimuthal angle of incoming antineutrino ray
//   Ebar      antineutrino energy
double fnubar(const double& theta, const double& phi, const double& E) {
  return fnu(theta,phi,E); // in this test implementation fnubar==fnu;
} // END Inline fnubar

// Total Integrand (divided by scale factor)
// f = fnu(theta,phi,E) * fnubar(thetabar,phibar,Ebar) * otherstuff(all6vars)
//   x       array of size dim (==6), giving coordinates {theta,thetabar,phi,phibar,E,Ebar}
//              x[0]=theta     polar angle of incoming neutrino ray
//              x[1]=thetabar  polar angle of incoming antineutrino ray
//              x[2]=phi       azimuthal angle of incoming neutrino ray
//              x[3]=phibar    azimuthal angle of incoming antineutrino ray
//              x[4]=E         neutrino energy
//              x[5]=Ebar      antineutrino energy
//   dim     number of dimensions in x
//   params  pointer to parameters for the function
double f(double* x, size_t dim, void* params) {
  if (dim != 6) {
    std::cout << "Error, dimension argument is not correct" << std::endl;
    return GSL_NAN; // error
  }
  const double trigterm = 1. - sin(x[0])*sin(x[1])*cos(x[2]-x[3]) - cos(x[0])*cos(x[1]);
  const double Eterm = (x[4]+x[5])*pow(x[4],3)*pow(x[5],3);
  const double fterm = fnu(x[0],x[2],x[4])*fnubar(x[1],x[3],x[5]);
  return fterm*trigterm*trigterm*Eterm;
} // END Inline f

// Print the result of a particular calculation
//   method       monte carlo technique used to obtain result
//   N            number of sampled points
//   result       value of the integral
//   sigma        error approximation to result
//   wallseconds  elapsed walltime for calculation
void print_result(const std::string method, const int N,
		  const double result, const double sigma,
		  const unsigned int wallseconds) {
  std::cout << method << " (N=" << N << ") =======================" << std::endl;
  std::cout << "  time   = " << wallseconds << " sec" << std::endl;
  std::cout << std::setprecision(16);
  std::cout << "  result = " << result << std::endl
	    << "  exact  = " << result_exact << std::endl
	    << "  sigma  = " << sigma << std::endl;
  std::cout << std::setprecision(3);
  std::cout << "  error  = " << fabs(result-result_exact)/sigma << " sigma" << std::endl;
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

  //   // sanity check
  //   double xcheck[6] = {pi,0.9*pi,0,0,1.,1.};
  //   std::cout << "integrand(pi,0.9pi,0,0,1,1) = " << f(xcheck,6,NULL) << std::endl;

  gsl_rng* rng = gsl_rng_alloc(gsl_rng_ranlxd2);
  unsigned int seed(time(NULL));
  gsl_rng_set(rng,seed);
  const size_t dim(6);
  // integration limits
  double x_low[dim]  = {0,0,0,0,0,0};
  double x_high[dim] = {pi,pi,2.*pi,2.*pi,11,11};
  gsl_monte_function F = {&f, dim, 0}; // convert the c++ function to type gsl_monte_function
  size_t N_calls = atoi(argv[1]); // number of function calls
  double result; // approximation to the integral
  double sigma; // approximation to the 1 stdev error

  unsigned int start_time; // seconds
  unsigned int elapsed_walltime;
  {
    std::string method("Plain");
    start_time = time(NULL);
    gsl_monte_plain_state *s = gsl_monte_plain_alloc(dim);
    gsl_monte_plain_integrate(&F, x_low, x_high, dim, N_calls, rng, s,
			      &result, &sigma);
    elapsed_walltime = time(NULL) - start_time;
    result *= scale; // scale up to correct dimension
    sigma *= scale;
    gsl_monte_plain_free(s); // free up the memory
    print_result(method, N_calls,
		 result, sigma,
		 elapsed_walltime);
  }
  

  {
    std::string method("Miser");
    start_time = time(NULL);
    gsl_monte_miser_state *s = gsl_monte_miser_alloc(dim);
    gsl_monte_miser_integrate(&F, x_low, x_high, dim, N_calls, rng, s,
			      &result, &sigma);
    elapsed_walltime = time(NULL) - start_time;
    result *= scale; // scale up to correct dimension
    sigma *= scale;
    gsl_monte_miser_free(s); // free up the memory
    print_result(method, N_calls,
		 result, sigma,
		 elapsed_walltime);
  }

  {
    std::string method("Vegas");
    start_time = time(NULL);
    gsl_monte_vegas_state *s = gsl_monte_vegas_alloc(dim);
    gsl_monte_vegas_integrate(&F, x_low, x_high, dim, N_calls, rng, s,
			      &result, &sigma);
    elapsed_walltime = time(NULL) - start_time;
    result *= scale; // scale up to correct dimension
    sigma *= scale;
    gsl_monte_vegas_free(s); // free up the memory
    print_result(method, N_calls,
		 result, sigma,
		 elapsed_walltime);
  }

  gsl_rng_free(rng); // free up the memory
  return 0;
}
