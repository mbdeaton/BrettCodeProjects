// FindRadialMode.cpp
// Astrophysical Fluid Dynamics: HW09
// Find a radial pulsation mode for a nonrotating star
//   Brett Deaton 12.6.11

#include"DataWriter.hpp"
#include"FindRoot.hpp"
#include"Function.hpp"

#include <cmath>
#include <cstdlib>
#include<iostream>
#include<sstream>
#include<vector>

/*************************************************************************/
void help(const std::string exec_name, const int num_expected_args) {
  std::cout << "ERROR! This executable requires " << num_expected_args 
	    << " argument at the command line." << std::endl
	    << "use: " << exec_name << " omega_first_guess" << std::endl
	    << "  omega_first_guess = First guess of oscillation mode frequency." << std::endl
	    << "                      G=c=1 units," << std::endl
	    << "                             1 radian/sec = 5*10^-6 code units" << std::endl
	    << "                             1 code unit  = 2*10^5 rad/sec" << std::endl
	    << "                      If 0, output will be a table of values of the" << std::endl
	    << "                      error equation, eqn(omega). Plot" << std::endl
	    << "                      the table and determine a first guess." << std::endl
	    << "                      If -1, output will be a table of values of the" << std::endl
	    << "                      perturbed stability eqn, u(omega)." << std::endl
	    << "example: "<< exec_name << " 1" << std::endl;
}

/*************************************************************************/
// Root find the nearest radial oscillation frequency given an initial guess
int main(int argc, char **argv) {
  const int num_expected_args = 1;
  if (argc!=num_expected_args+1) {
    help(argv[0],num_expected_args);
    return 1;
  }
  const double omega_0 = atof(argv[1]);
  const double Gamma = 2; // polytropic index
  const int res = 10000; // integration resolution
  const double error_threshold = 1e-5;  
  
  // output an error table instead of solve for omega
  if (omega_0==0) {
    const int nsteps = 100;
    const double omega_min = 1;
    const double omega_max = 100;
    const double h = (omega_max-omega_min)/(nsteps-1);
    const std::string file_name("error.dat");
    std::stringstream header;
    header << "# Output from " << argv[0] << " " << argv[0] << file_name << std::endl
 	   << "# A plot of the error function as a function of omega." << std::endl
 	   << "# Find the zeros visually; these ar the eigenmodes." << std::endl
 	   << "# omega stepsize  = " << nsteps << std::endl
 	   << "# omega min       = " << omega_min << std::endl
 	   << "# omega max       = " << omega_max << std::endl
 	   << "# [1] omega" << std::endl
 	   << "# [2] error" << std::endl
 	   << "# [3] derror/domega" << std::endl;
    OneDimDataWriter fout(file_name,header.str());
    double omega = omega_min;
    for (int i=0; i<nsteps; ++i) {
      SurfaceConstraint soln(Gamma,error_threshold,res);
      const double error = soln.f(omega);
      const double derror = soln.df(omega);
      fout.AppendToFile(omega,error,derror);
      omega += h;
    }
  }
  // output a perturbation table instead of solve for omega
  else if (omega_0==-1) {
    double omega = 2.187; // f-mode frequency for this star
    RadialOscillationSolutionForGamma2Polytrope soln(omega,res);
    const double M = soln.GetMass();
    const double R = soln.GetFinalr();
    const std::string file_name("perturb.dat");
    std::stringstream header;
    header << "# Output from " << argv[0] << " " << argv[0] << file_name << std::endl
 	   << "# A plot of the perturbation as a function of radius." << std::endl
 	   << "# omega  = " << omega << std::endl
	   << "# mass   = " << M << std::endl
 	   << "# r_min  = " << 0 << std::endl
 	   << "# r_max  = " << R << std::endl
 	   << "# [1] r" << std::endl
 	   << "# [2] u = zeta_r/r" << std::endl
 	   << "# [3] du/dr" << std::endl;
    OneDimDataWriter fout(file_name,header.str());
    const std::vector<double> u(soln.GetProfileu());
    const std::vector<double> dudr(soln.GetProfiledudr());
    const double h = R/(double)res;
    for (int i=0; i<res; ++i) {
      const double r = i*h;
      fout.AppendToFile(r,u[i],dudr[i]);
    }
  }
  // normal output
  else {
    FunctionAndDeriv* pSurfaceError = new SurfaceConstraint(Gamma,error_threshold,res);
    const double omega = RtSolve(pSurfaceError,omega_0,error_threshold);
    std::cout << "omega = " << omega <<  std::endl;
  }
  return 0;
} // END main
  
