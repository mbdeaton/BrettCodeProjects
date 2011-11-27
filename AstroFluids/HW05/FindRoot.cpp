// Root Finding & Function Structure from Brett Deaton 7.09.10
// Solving the Shock tube problem from Astro Fluids 10.7.11

#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<math.h>
#include<fstream>
#include<iostream>
#include<sstream>
#include<iomanip>

/**************************************************************************************/
// Append to a DataFile
class OneDimDataWriter {
public:
  /*Constructor:
      file_nananme        name of output file including extension
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
  const void AppendToFile(const double& x, const double& y, const double& z) {
    mOut << x << "   " << y << "   " << z << std::endl;
  };
private:
  std::ofstream mOut;
}; // END Class OneDimDataWriter

/**************************************************************************************/
// Function Class
// Abstract base class
// Members:
//   f  - the function itself
//   df - the first derivative of the function
class Function {
public:  
  //  Function() {}
  virtual ~Function() {}
  virtual double f(double x) const =0;
  virtual double df(double x) const =0;
}; // END Class Function

/**************************************************************************************/
// VelocityFunction
// velocity to the right and left of a 1D shock
class VelocityFunction : public Function {
public:
  VelocityFunction() :
    rhoL(1.), PL(1.), vL(0.),
    rhoR(0.125), PR(0.1), vR(0),
    Gamma(1.4), csL(sqrt(Gamma*PL/rhoL)), AR(2/(Gamma+1)/rhoR), BR((Gamma-1)*PR/(Gamma+1)) {}
  ~VelocityFunction() {}
  double f(double Pstar) const {
    double bigtermL = 2*csL*(pow(Pstar/PL,(Gamma-1)/2/Gamma)-1)/(Gamma-1);
    double bigtermR = (Pstar-PR)*sqrt(AR/(Pstar+BR));
    return vL - bigtermL - vR - bigtermR;
  }
  double df(double Pstar) const {
    double bigtermL = csL/Gamma*pow(PL,(1-Gamma)/2/Gamma)*pow(Pstar,(-Gamma-1)/2/Gamma);
    double bigtermR1 = sqrt(AR/(Pstar+BR));
    double bigtermR2 = (Pstar-PR)/2*sqrt(AR/pow(Pstar+BR,3));
    return -bigtermL - bigtermR1 + bigtermR2;
  }
private:
  const double rhoL, PL, vL, rhoR, PR, vR, Gamma, csL, AR, BR;
}; // END Class Function:VelocityFunction

/**************************************************************************************/
// Root Solve using the Newton-Raphson method
// Arguments:
//   myFunc   - pointer to a Function object
//   x        - first ordinal guess
//   eps      - epsilon: maximum error bound desired (on the x-axis)
// note: not very robust
double RtSolve(const Function *myFunc, double x, const double eps) {
  double rt = x - (myFunc->f(x)) / (myFunc->df(x));
  int iter = 0, maxiter=100000;
  bool debug = true;
  while (fabs(rt-x)>eps && iter<maxiter) {
    if (debug) std::cout << "  Pstar_" << iter << " = " << x << std::endl;
    x = rt;
    rt = x - (myFunc->f(x)) / (myFunc->df(x));
    // std::cout << x << endl;
    ++iter;
  }
  if (iter==maxiter) {
    std::cout << "ERROR: RtSolve reached max iterations; unable to find root within desired accuracy." << std::endl;
  }
  return rt;
} // END Inline RtSolve

/**************************************************************************************/
void help(const std::string exec_name, const int num_expected_args) {
  std::cout << "ERROR! This executable requires " << num_expected_args
            << " arguments at the command line." << std::endl
            << "use: " << exec_name << " <double guess> <int plotflag>" << std::endl
            << "  guess    = first guess of the root for the newton raphson solver" << std::endl
            << "  plotflag = 0 if no plot is desired, nonzero if plot is desired" << std::endl
            << "example: "<< exec_name << " 0.2 0" << std::endl;
} // END Inline help

/**************************************************************************************/
int main(int argc, char **argv) {
  const int num_expected_args = 2;
  if (argc!=num_expected_args+1) {
    help(argv[0],num_expected_args);
    return 1;
  }

  Function * velocity = new VelocityFunction;

  bool print_velocity_function_and_deriv = atoi(argv[2]);
  if (print_velocity_function_and_deriv) {
    const std::string vfilename = "findroot.dat";
    std::stringstream vheader;
    vheader << "# Output from " << argv[0] << std::endl <<
      "#   a plot of the velocity function & deriv on [0,1]" << std::endl << 
      "# [1] Pstar" << std::endl << 
      "# [2] vfunction" << std::endl <<
      "# [3] deriv vfunction" << std::endl;
    OneDimDataWriter vout(vfilename,vheader.str());
    const double lowerx=0, upperx=1, N=100, dx=(upperx-lowerx)/(N-1);
    for (int i=0; i<N; ++i) {
      vout.AppendToFile(lowerx+i*dx,
			velocity->f(lowerx+i*dx),
			velocity->df(lowerx+i*dx));
    }
  } // END if (print_velocity_function_and_deriv)

  // find root
  double Pstar_0 = atof(argv[1]); // initial guess
  const double eps = 1.e-6;
  std::cout << "Root Find for Velocity Function ---------------------" << std::endl;
  double Pstar = RtSolve(velocity,Pstar_0,eps);
  std::cout << "  root at Pstar = " << Pstar << std::endl;
  
  delete velocity;
  return 0;
} // END main
