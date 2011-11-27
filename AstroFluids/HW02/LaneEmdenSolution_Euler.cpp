// LaneEmdenSolution_Euler.cpp
// Astrophysical Fluid Dynamics: HW02
//   Numerically integrates (using Euler) the Nondimensionalized Lane-Emden Eqn
//   Brett Deaton 9.10.11

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

/*************************************************************************/
// Idea sort of stolen from SpEC Class 2011 (Harald Pfeiffer) because this is a useful class
//   to copy and paste into future assignments.
class OneDimDataWriter {
public:
  /* Constructor:
     file_name        name of output file including extension
     header           any comment to be included at the top of the file,
                      w/ comment char incl, but not the final endl
  */
  OneDimDataWriter(const std::string file_name,
		   const std::string header):
    mOut(file_name.c_str()) { mOut << header << std::endl;};
  ~OneDimDataWriter() { mOut.close();};
  /* AppendToFile
     x   abscissa
     y   ordinate
  */
  const void AppendToFile(const double& x, const double& y) {
    mOut << x << "   " << y << std::endl;
  };
private:
  std::ofstream mOut;
}; // END class OneDimDataWriter

/*************************************************************************/
void help(const std::string exec_name, const int num_expected_args) {
  std::cout << "ERROR! This executable requires " << num_expected_args 
	    << " arguments at the command line." << std::endl
	    << "use: " << exec_name << " <int res> <double poly_n> <string filename>" << std::endl
	    << "  res      = integration stepsize in zeta is pi/res" << std::endl
	    << "  poly_n   = polytropic index defined by P=kappa*rho^{1+1/n}" << std::endl
	    << "  filename = name of output dat file w/ extension" << std::endl
	    << "example: "<< exec_name << " 1000 1 out.dat" << std::endl;
}

/*************************************************************************/
// Solves the nondimensionalized Lane-Emden Eqn:
//   zeta^{-2} * d_{zeta}[zeta^2*d_{zeta}[theta]] = -theta^n
//   Outputs a table of [zeta,theta]
// 2nd order Lane-Emden eqn becomes 2 first order eqns:
//   1) d_zeta[theta] = u
//   2) d_zeta[u]     = -theta^n - 2*u/zeta
// Translating to 1st order Euler algorithm, stepsize h, gives:
//   1) theta_{i+1} = theta_i + h*u_i
//   2) u_{i+1}     = u_i - h*theta_i^n - 2*u_i/i
int main(int argc, char **argv) {
  const int num_expected_args = 3;
  if (argc!=num_expected_args+1) {
    help(argv[0],num_expected_args);
    return 1;
  }

  // set up
  const double pi = 4.*atan(1);
  const std::string file_name = argv[3];
  const double poly_n = atof(argv[2]);
  const int res = atoi(argv[1]);
  const double h = pi/res;
  double theta = 1; // initial central density
  double u = 0; // initial slope of density in zeta
  double zeta = h;
  std::stringstream mheader;
  mheader << "# Output from " << argv[0] << std::endl <<
    "#   zeta stepsize = " << h << std::endl <<
    "#[1] zeta\n#[2] theta(zeta)" << std::endl;
  OneDimDataWriter plotout(file_name,mheader.str());

  // Print info to stdout
  std::cout << "Finding stellar profile theta(zeta) with nondimensional Lane-Emden eqn." << std::endl
	    << "  Integration method: Euler 1st order" << std::endl
	    << "  polytropic index  = " << poly_n <<  std::endl
	    << "  central theta     = " << theta <<  std::endl
	    << "  stepsize          = " << h <<  std::endl;

  // compute and print theta profile
  plotout.AppendToFile(zeta,theta);
  int i = 1;
  const int maxi = 100000; // keeps us from infinite loops due to poly_n >= 5;
  while(theta>0 && i<maxi) {
    theta = theta + h*u;
    u = u - h*pow(theta,poly_n) - 2*u/i;
    ++i;
    zeta = i*h;
    plotout.AppendToFile(zeta,theta);
  }

  if(theta>0) {
    std::cout << "ERROR! Integrated max number of steps (" << maxi << ") and terminated." << std::endl
	      << "  Hint: Theta never vanishes for poly_n >= 5" << std::endl;
    return 1;
  }

  std::cout << "  surface found at  = " << zeta <<  std::endl;
  return 0;
}
