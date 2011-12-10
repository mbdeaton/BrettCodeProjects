// RadialOscillationSolutionForGamma2Polytrope.hpp
// Astrophysical Fluid Dynamics: HW09
// Find a solution to the radial oscillation equation, for a Gamma=2 polytrope,
//   which has a density profile proportional to sin(r)/r
// Brett Deaton 12.6.11

#include<cmath>
#include<iostream>
#include<vector>

/*************************************************************************/
// Class RadialOscillationSolutionForGamma2Polytrope
//
// Solves the stellar radial oscillation equation (one fourier component, omega)
//   L[u] = omega^2*u
//     in which:
//     L[u] = - d_r[Gamma*P*r^4*d_r[u]]/rho/r^4 - 
//     u = zeta_r/r
//     zeta = lagrangian perturbation variable
//     Gamma = polytropic constant
//
// Assumes the following stellar profile (the solution to the Lane-Emden
//   equation with Gamma=2):
//   rho(r) = sin(r)/r
//   P(r) = 2*pi*rho^2
//
// Does not apply the appropriate boundary condition at r=R=pi, simply integrates
//   to r=pi (actually, to r=pi-h, to avoid a singularity at r=pi). To use this class
//   for a correct solution, supply one of the mode frequencies, omega_n exactly.
//   Otherwise, wrap the class in a root-finder, and use the shooting method to find
//   the mode frequency in question.
//
// Arguments
//   omega    = frequency of the mode
//   res      = number of integration gridpoints in r; h=pi/res
// 
// Makes a table of {r,u,d_r[u]} on r=(0,pi-h)
class RadialOscillationSolutionForGamma2Polytrope {
public:
  RadialOscillationSolutionForGamma2Polytrope(const double& omega, const int& res) :
    momega(omega),
    mpi(4.*atan(1)),
    mGamma(2), // this doesn't have to be same as stellar structure Gamma
    mh(mpi/(double)res),
    mM(4*mpi*mpi),
    mres(res),
    msolution(2,std::vector<double>(res)) {
    Integrateu();
  }; // END constructor
  ~RadialOscillationSolutionForGamma2Polytrope() {};
  double GetMass() { return mM; };
  double GetFinalr() { return mpi-mh; };
  double GetFinalu() { return msolution[0][mres-1]; };
  double GetFinaldudr() { return msolution[1][mres-1]; };
  std::vector<double> GetProfileu() { return msolution[0]; };
  std::vector<double> GetProfiledudr() { return msolution[1]; };
  
private:
  void Integrateu() {
    double u = 1; // initial central u
    double v = 0; // initial slope of u; v=d_r[u]
    double r = 0;
    double j1,j2,j3,j4; // RK4 constants for u
    double k1,k2,k3,k4; // RK4 constants for v
    int i = 0;
    msolution[0][i] = u;
    msolution[1][i] = v;
    while(i<mres) {
      r = i*mh;
      j1 = mh*g(u,v,r);
      k1 = mh*f(u,v,r);
      j2 = mh*g(u+j1/2.,v+k1/2.,r+mh/2.);
      k2 = mh*f(u+j1/2.,v+k1/2.,r+mh/2.);
      j3 = mh*g(u+j2/2.,v+k2/2.,r+mh/2.);
      k3 = mh*f(u+j2/2.,v+k2/2.,r+mh/2.);
      j4 = mh*g(u+j3,v+k3,r+mh);
      k4 = mh*f(u+j3,v+k3,r+mh);
      u += (j1+2.*j2+2.*j3+j4)/6.;
      v += (k1+2.*k2+2.*k3+k4)/6.;
      ++i;
      msolution[0][i] = u;
      msolution[1][i] = v;
    }
  }; // END Integrateu
  // d_r[v]=f(u,v,r)
  double f(const double& u, const double& v, const double& r) {
    double mr(r);
    if (r==0) mr=mh/100.; // to avoid singularity at r==0
    const double a = 4.*mpi*u*(3.*mGamma-4.)*(sin(mr)/mr-cos(mr))/mGamma/sin(mr)/mr;
    const double b = 2.*v*(sin(mr)/mr+cos(mr))/sin(mr);
    const double c = mr*momega*momega*u/mGamma/sin(mr);
    //     std::cout << "ro:a = " << a << std::endl;
    //     std::cout << "ro:4.*mpi*u*(3.*mGamma-4.) = " << 4.*mpi*u*(3.*mGamma-4.) << std::endl;
    //     std::cout << "ro:sin(mr)/mr-cos(mr) = " << sin(mr)/mr-cos(mr) << std::endl;
    //     std::cout << "ro:1/mGamma/sin(mr)/mr" << 1/mGamma/sin(mr)/mr << std::endl;
    //     std::cout << "ro:r = " << mr << std::endl;
    //     std::cout << "ro:f = " << a - b - c << std::endl;
    return a - b - c;
  };
  // d_r[u]=g(u,v,r)
  double g(const double& u, const double& v, const double& r) {
    return v;
  };
  const double momega;
  const double mpi, // the well-beloved constant
    mGamma, // polytropic index for adiabatic perturbations
    mh, // stepsize for integration
    mM; // mass of the star
  const int mres; // resolution for the integration
  std::vector<std::vector<double> > msolution; // msolution[0][i] = u(r=i*h)
  //                                              msolution[1][i] = d_r[u](r=i*h)
}; // END class RadialOscillationSolutionForGamma2Polytrope
