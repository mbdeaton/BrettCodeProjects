// Brett Deaton 12.9.1l
// Function Class

#ifndef Function_H
#define Function_H

#include"RadialOscillationSolutionForGamma2Polytrope.hpp"
#include<iostream>
#include<math.h>


/**************************************************************************************/
// Abstract base class
// Members:
//   f  - the function itself
//   df - the function's first derivative
class FunctionAndDeriv {
public:
  virtual ~FunctionAndDeriv() {};
  virtual double f(const double& x) const =0;
  virtual double df(const double& x) const =0;
}; // END Class FunctionAndDeriv

/**************************************************************************************/
// SurfaceConstraint
// The surface boundary condition for the Radial Stellar Oscillation Equation is
//   SurfaceConstraint = 0
//   SurfaceConstraint = d_r[u] - stuff
// It's a function of the frequency
class SurfaceConstraint : public FunctionAndDeriv {
public:
  // Gamma   = adiabatic gas constant
  // eps     = error threshold being used for root finding
  // res     = resolution to use for the integrations of the RadialOscillationsSolutions
  SurfaceConstraint(const double& Gamma, const double& eps, const int& res):
    mGamma(Gamma), meps(eps), mG(1.), mres(res) {;};
  ~SurfaceConstraint() {;};
  double f(const double& omega) const {
    RadialOscillationSolutionForGamma2Polytrope soln(omega,mres);
    const double R = soln.GetFinalr();
    const double M = soln.GetMass();
    const double u = soln.GetFinalu();
    const double dudr = soln.GetFinaldudr();
    // std::cout << "sc:f = " << dudr - u*(omega*omega*R*R*R/mG/M-(3.*mGamma-4.))/mGamma/R << std::endl;
    return dudr - u*(omega*omega*R*R*R/mG/M-(3.*mGamma-4.))/mGamma/R;
  };
  double df(const double& omega) const {
    // compute a numerical deriv
    RadialOscillationSolutionForGamma2Polytrope solnii(omega,mres);
    const double R = solnii.GetFinalr();
    const double M = solnii.GetMass();
    const double domega = 0.001;
    double u = solnii.GetFinalu();
    //     std::cout << "final u    = " << u << std::endl;
    double dudr = solnii.GetFinaldudr();
    //     std::cout << "final dudr = " << dudr << std::endl;
    const double fii = dudr - u*(omega*omega*R*R*R/mG/M-(3.*mGamma-4.))/mGamma/R;
    RadialOscillationSolutionForGamma2Polytrope solni(omega-domega,mres);
    u = solni.GetFinalu();
    dudr = solni.GetFinaldudr();
    const double fi = dudr - u*((omega-domega)*(omega-domega)*R*R*R/mG/M-(3.*mGamma-4.))/mGamma/R;
    //     std::cout << "sc:R = " << R << std::endl;
    //     std::cout << "sc:M = " << M << std::endl;
    //     std::cout << "sc:omega = " << omega << std::endl;
    //     std::cout << "sc:domega = " << domega << std::endl;
    //     std::cout << "sc:ui = " << solni.GetFinalu() << std::endl;
    //     std::cout << "sc:dui = " << solni.GetFinaldudr() << std::endl;
    //     std::cout << "sc:uii = " << solnii.GetFinalu() << std::endl;
    //     std::cout << "sc:duii = " << solnii.GetFinaldudr() << std::endl;
    //     std::cout << "sc:df = " << (fii-fi)/domega << std::endl;
    return (fii-fi)/domega;
  };
private:
  const double mGamma, // polytropic index for adiabatic perturbations
    meps, // error threshold being used for root finding
    mG; // gravitational constant
  const int mres; // resolutions to use in radial integrations
}; // END Class SurfaceConstraint : FunctionAndDeriv

#endif
