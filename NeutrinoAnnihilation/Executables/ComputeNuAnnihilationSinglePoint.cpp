// Integrating the neutrino-antineutrino annihilation energy deposition at a single point.
// Brett Deaton - Feb 2012

#include <cstdlib>
#include <string>

#include "Utils/ErrorHandling/Require.hpp"

class NuAnnihilationIntegral;
class NAIMiser;
void NAIMiser(const double, const double, const double*);
class NAIVegas;
void NAIVegas(const double, const double, const double*);
class NuDistributionFunction;
class NDFPiecewiseConstant;
class NDFSeparableAnalytic;
class NDFRayTraceNu;

// TODO (brett): get rid of these assumptions about the input grid, accuracy, etc.
// global vars
const double g_xmax(100), g_kymax(100), g_zmax(60); // volume assumed symmetric about origin (km)
const double g_best_accuracy(1e-6); // best fractional accuracy (sigma_Q/Q) currently supported
const double g_min_Escale(), g_max_Escale(100); // largest and smallest energy scale expected (MeV)

void help(const std::string exec_name, const int num_expected_args) {
  std::cout << "Usage error. This executable requires " << num_expected_args
            << " arguments at the command line. Thus:" << std::endl
            << exec_name << " method x y z Escale accuracy" << std::endl
            << "  method     string, integration method: \'-miser\' or \'-vegas\'" << std::endl
            << "  x y z      double double double, compute integral at (x,y,z) in km" << std::endl
	    << "  Escale     double, approximate max energy of neutrinos in MeV" << std::endl
            << "  accuracy   double, target percent accuracy of integral" << std::endl
            << "example: "<< exec_name << " -vegas 0 0 50 10 0.01" << std::endl
	    << "Prints Q, heating rate per volume in erg/cm^3/s" << std::endl;
} // END Inline help

int main (int argc, char** argv) {
  const int num_expected_args(6);
  if (argc!=num_expected_args+1) {
    help(argv[0],num_expected_args);
    return 1;
  }

  const std::string method(argv[1]);
  const double x[3] = {atof(argv[2]), atof(argv[3]), atof(argv[4])}; // {x,y,z} in km
  const double Escale(atof(argv[5]));
  const double accuracy(atof(argv[6]));

  // Sanity Checks
  // accuracy out of range will cause prohibitively too many or too few rays to be cast.
  REQUIRE((accuracy<g_best_accuracy) | (accuracy>1),
	  "Requested accuracy is too large or too small.");
  // Escale out of range is a sign that the client has misunderstood this argument.
  REQUIRE((Escale<g_min_Escale) | (Escale>g_max_Escale),
	  "Neutrino energy scale is too large or small.");
  REQUIRE((x[0]<-g_xmax) | (x[0]>g_xmax) |
	  (x[1]<-g_ymax) | (x[1]>g_ymax) |
	  (x[2]<-g_zmax) | (x[2]>g_zmax)
	  "Point x,y,z is outside the integrable domain.");

  // set up integral
  NuAnnihilationIntegral* integral;
  if (method=="-miser") {
    integral = new NAIMiser(accuracy,Escale,x);
  }
  else if(method=="-vegas") {
    integral = new NAIVegas(accuracy,Escale,x);
  }
  else {
    REQUIRE(false,"Integration method not recognized.");
  }

  integral->PrintResult();

  delete integral;
  return EXIT_SUCCESS;
}
