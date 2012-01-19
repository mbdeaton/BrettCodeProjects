// Test program to integrate a 6D function. Try integrating
//   f(\vec{x}) = \product_{i=0}^{5} {cos(x_i)}
//   \int_{0}^{pi/2} {dx_0} \int_{0}^{pi/2} {dx_1} ... \int_{0}^{pi/2} {dx_5} f(\vec{x}) == 1

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
const double result_exact = 1.0;

// f(\vec{x}) = cos(x_0)*cos(x_1)*...*cos(x_5)
//   x       array of size dim (==6), giving coordinates {\vec{x}}
//   dim     number of dimensions in x
//   params  pointer to parameters for the function
double f(double* x, size_t dim, void* params) {
  if (dim == 6) {
    return cos(x[0])*cos(x[1])*cos(x[2])*cos(x[3])*cos(x[4])*cos(x[5]);
  }
  std::cout << "Error, dimension argument is not correct" << std::endl;
  return GSL_NAN; // error
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

  gsl_rng* rng = gsl_rng_alloc(gsl_rng_ranlxd2);
  unsigned int seed(time(NULL));
  gsl_rng_set(rng,seed);
  const size_t dim(6);
  // integration limits
  double x_low[dim]  = {0,0,0,0,0,0};
  double x_high[dim] = {pi/2.,pi/2.,pi/2.,pi/2.,pi/2.,pi/2.};
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
    gsl_monte_vegas_free(s); // free up the memory
    print_result(method, N_calls,
		 result, sigma,
		 elapsed_walltime);
  }

  gsl_rng_free(rng); // free up the memory
  return 0;
}
