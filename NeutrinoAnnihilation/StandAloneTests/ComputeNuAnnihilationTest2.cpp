// Integrating the neutrino-antineutrino annihilation energy deposition at a point
//   in space given the distribution functions f_nu(theta,phi,E), f_nubar(theta,phi,E).
//   Here I assume the nu and nubar distribution functions are proportional.

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
const double result_exact_scaled = 0.6522; // from this integration with n=1e7, scaled so
//                                            this*HarikaeConst*(Escale^9) is in erg/cm^3/s
// params for my test function
const double twidth = pi/4.;
const double numwobbles = 5; // must be an integer so that f(phi) is continuous
const double wobblescale = 0.3; // must be between 0 and 1
const double thermalscale = 0.05*Escale; // k_B*T, scaled
const double fermilevel = Ebreak;

// Neutrino distribution function (h^{-3} factored out, but not Escale)
// fnu(theta,phi,E)
//   theta  polar angle of incoming neutrino ray
//   phi    azimuthal angle of incoming neutrino ray
//   E      neutrino energy in erg, not normalized, that's done in f
double fnu(const double& theta, const double& phi, const double& E) {
  const double fnut = exp(-(theta-pi)*(theta-pi)/twidth/twidth);
  const double fnup = 1. + wobblescale*sin(numwobbles*phi);
  const double fnue = 1./(1.+exp((E-fermilevel)/thermalscale));
  return fnut*fnup*fnue;
} // END Inline fnu

// AntiNeutrino distribution function
// fnubar(theta,phi,E)
//   thetabar  polar angle of incoming antineutrino ray
//   phibar    azimuthal angle of incoming antineutrino ray
//   Ebar      antineutrino energy, not normalized, that's done in f
double fnubar(const double& thetabar, const double& phibar, const double& Ebar) {
  return 2.*fnu(thetabar,phibar,Ebar); // in this test implementation fnubar==2*fnu;
} // END Inline fnubar

// Total Integrand (divided by HarikaeConst, and scaled by Escale^7; the other two factors of
//   Escale come from dE and dEbar).
// f = fnu(theta,phi,Escaled*Escale) * fnubar(thetabar,phibar,Ebarscaled*Escale) * otherstuff
//   x       array of size dim (==6), giving coordinates {theta,thetabar,phi,phibar,E,Ebar}
//              x[0]=theta        polar angle of incoming neutrino ray
//              x[1]=thetabar     polar angle of incoming antineutrino ray
//              x[2]=phi          azimuthal angle of incoming neutrino ray
//              x[3]=phibar       azimuthal angle of incoming antineutrino ray
//              x[4]=Escaled      neutrino energy
//              x[5]=Ebarscaled   antineutrino energy
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
  std::cout << "  result                 = " << result << std::endl
	    << "  exact (not very exact) = " << result_exact_scaled << std::endl
	    << "  sigma                  = " << sigma << std::endl;
  std::cout << std::setprecision(3);
  std::cout << "  error (not meaningful) = " << fabs(result-result_exact_scaled)/sigma << " sigma" << std::endl;
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

  //   // sanity checks
  //   std::cout << "Ebreak = " << Ebreak << std::endl;
  //   std::cout << "Escale = " << Escale << std::endl;
  //   std::cout << "fermilevel = " << fermilevel << std::endl;
  //   std::cout << "HarikaeConst = " << HarikaeConst << std::endl;
  //   double xcheck[6] = {pi,0.9*pi,0,0,1.1,1.1};
  //   std::cout << "integrand(pi,0.9pi,0,0,11MeV/Escale,11MeV/Escale) = "
  // 	    << f(xcheck,6,NULL) << " (should be 0.000226092)" << std::endl;
  
  gsl_rng* rng = gsl_rng_alloc(gsl_rng_ranlxd2);
  unsigned int seed(time(NULL));
  gsl_rng_set(rng,seed);
  const size_t dim(6);
  // integration limits
  double x_low[dim]  = {0,0,0,0,0,0};
  double x_high[dim] = {pi,pi,2.*pi,2.*pi,1.5,1.5};
  gsl_monte_function F = {&f, dim, 0}; // convert the c++ function to type gsl_monte_function
  size_t N_calls = atoi(argv[1]); // number of function calls
  double result; // monte carlo estimate of the integral
  double sigma; // monte carlo estimate of the 1 stdev error

  unsigned int start_time; // seconds
  unsigned int elapsed_walltime;
  const bool only_perform_vegas(true);

  if (!only_perform_vegas) {
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

  if (!only_perform_vegas) {
    std::string method("Miser");
    start_time = time(NULL);
    gsl_monte_miser_state *s = gsl_monte_miser_alloc(dim);
    gsl_monte_miser_integrate(&F, x_low, x_high, dim, N_calls, rng, s,
			      &result, &sigma);
    elapsed_walltime = time(NULL) - start_time;
    gsl_monte_miser_params params;
    gsl_monte_miser_params_get(s,&params);
    //    std::cout << "size of state (bytes) = " << sizeof(*s) << std::endl;
    gsl_monte_miser_free(s); // free up the memory
    print_result(method, N_calls,
		 result, sigma,
		 elapsed_walltime);
    // print correctly scaled result
    std::cout << "  Q = " << result*HarikaeConst*pow(Escale,9)
              << " +- " << sigma*HarikaeConst*pow(Escale,9) << std::endl;
    // sanity check on integration params
    std::cout << "params.estimate_frac = " << params.estimate_frac << std::endl;
    std::cout << "params.min_calls = " << params.min_calls << std::endl;
    std::cout << "params.min_calls_per_bisection = " << params.min_calls_per_bisection << std::endl;
    std::cout << "params.alpha = " << params.alpha << std::endl;
    std::cout << "params.dither = " << params.dither << std::endl;
  }

  FILE* psamples_file;
  psamples_file = fopen("sample_points.txt", "w+");
  //  fprintf(psamples_file,"Hello world.\n");
  {
    std::string method("Vegas");
    start_time = time(NULL);
    gsl_monte_vegas_state *s = gsl_monte_vegas_alloc(dim);
    gsl_monte_vegas_params params;
    // sanity check on integration params before integration
    gsl_monte_vegas_params_get(s,&params);
    std::cout << "pre params.alpha = " << params.alpha << std::endl;
    std::cout << "pre params.iterations = " << params.iterations << std::endl;
    std::cout << "pre params.stage = " << params.stage << std::endl;
    std::cout << "pre params.mode = " << params.mode << std::endl;
    std::cout << "pre params.verbose = " << params.verbose << std::endl;
    std::cout << "pre params.ostream = " << params.ostream << std::endl;
    params.verbose = 2; // -1   no output
    //                      0   summary info
    //                      1   + sample points
    //                      2   + rebinning at each iteration
    params.ostream = psamples_file;
    gsl_monte_vegas_params_set(s,&params);
    gsl_monte_vegas_integrate(&F, x_low, x_high, dim, N_calls, rng, s,
			      &result, &sigma);
    // sanity check on integration params after integration
    std::cout << "post params.alpha = " << params.alpha << std::endl;
    std::cout << "pre params.iterations = " << params.iterations << std::endl;
    std::cout << "post params.stage = " << params.stage << std::endl;
    std::cout << "post params.mode = " << params.mode << std::endl;
    std::cout << "post params.verbose = " << params.verbose << std::endl;
    std::cout << "post params.ostream = " << params.ostream << std::endl;
    fclose(psamples_file);
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
