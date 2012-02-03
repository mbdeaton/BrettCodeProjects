// Test program to see if I can use & link gsl libraries correctly
//   and how integration by the two techniques works. Try integrating
//   f(x,y)=cos(x)*cos(y)
//   \int_{0}^{pi/2} {dx} \int_{0}^{pi/2} {dy} f(x,y) == 1

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

// f(x,y) = cos(x)*cos(y)
//   x       array of size dim (==2), giving coordinates {x,y}
//   dim     number of dimensions in x
//   params  pointer to parameters for the function
double f(double* x, size_t dim, void* params) {
  if (dim == 2) {
    return cos(x[0])*cos(x[1]);
  }
  return GSL_NAN; // error
} // END Inline f

// Print the result of a particular calculation
//   method    monte carlo technique used to obtain result
//   N         number of sampled points
//   result    value of the integral
//   error     error approximation to result
void print_result(const std::string method, const int N,
		  const double result, const double sigma) {
  std::cout << method << " (N=" << N << ") =======================" << std::endl;
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
  const size_t dim(2);
  // integration limits
  double x_low[dim]  = {0,0};
  double x_high[dim] = {pi/2.,pi/2.};
  gsl_monte_function F = {&f, dim, 0}; // convert the c++ function to type gsl_monte_function
  size_t N_calls = atoi(argv[1]); // number of function calls
  double result; // approximation to the integral
  double sigma; // approximation to the 1 stdev error

  {
    std::string method("Plain");
    gsl_monte_plain_state *s = gsl_monte_plain_alloc(dim);
    gsl_monte_plain_integrate(&F, x_low, x_high, dim, N_calls, rng, s,
			      &result, &sigma);
    gsl_monte_plain_free(s); // free up the memory
    print_result(method, N_calls,
		 result, sigma);
  }

  {
    std::string method("Miser");
    gsl_monte_miser_state *s = gsl_monte_miser_alloc(dim);
    gsl_monte_miser_integrate(&F, x_low, x_high, dim, N_calls, rng, s,
			      &result, &sigma);
    gsl_monte_miser_free(s); // free up the memory
    print_result(method, N_calls,
		 result, sigma);
  }

  {
    std::string method("Vegas");
    gsl_monte_vegas_state *s = gsl_monte_vegas_alloc(dim);
    gsl_monte_vegas_integrate(&F, x_low, x_high, dim, N_calls, rng, s,
			      &result, &sigma);
    gsl_monte_vegas_free(s); // free up the memory
    print_result(method, N_calls,
		 result, sigma);
  }

  gsl_rng_free(rng); // free up the memory
  return 0;
}
