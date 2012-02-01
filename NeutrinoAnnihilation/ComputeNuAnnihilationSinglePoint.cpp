// Integrating the neutrino-antineutrino annihilation energy deposition at a single point
//   in space.
// Brett Deaton - Feb 2012

#include<cstdlib>
#include<ctime>
#include<iostream>
#include<iomanip>
#include<cmath>
#include<string>

class NuAnnihilationIntegral;
class NAIMiser;
void NAIMiser(const double, const double, const double*);
class NAIVegas;
void NAIVegas(const double, const double, const double*);
class NuDistributionFunction;
class NDFPiecewiseConstant;
class NDFSeparableAnalytic;
class NDFRayTraceNu;

// global vars
const double xmax(100), ymax(100), zmax(60); // volume assumed to be symmetric about origin (km)

void printerr(const std::string error_message) {
  std::cout << "** Error. " << error_message << std::endl;
} // END Inline printerr

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

  // sanity checks
  if ((Escale<0.1) | (Escale>100)) {
    printerr("Neutrino energy scale seems too large or small.");
    help(argv[0],num_expected_args);
    return 1;
  }
  if ((accuracy<1e-6) | (accuracy>1)) {
    printerr("Requested accuracy is too large or too small.");
    help(argv[0],num_expected_args);
    return 1;
  }
  if ((x[0]<-xmax) | (x[0]>xmax) |
      (x[1]<-ymax) | (x[1]>ymax) |
      (x[2]<-zmax) | (x[2]>zmax)) {
    printerr("Point x,y,z is outside the integrable domain.");
    help(argv[0],num_expected_args);
    return 1;
  }

  // set up integral
  NuAnnihilationIntegral* integ;
  if (method=="-miser") {
    integ = new NAIMiser(accuracy,Escale,x);
  }
  else if(method=="-vegas") {
    integ = new NAIVegas(accuracy,Escale,x);
  }
  else {
    printerr("Integration method not recognized.");
    help(argv[0],num_expected_args);
  }

  integ->printresult();

  delete integ;
  return 0;
}
