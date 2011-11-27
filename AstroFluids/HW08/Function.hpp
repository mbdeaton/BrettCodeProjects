// Brett Deaton 10.26.1l
// Function Class

#ifndef Function_H
#define Function_H

#include<math.h>

/**************************************************************************************/
// Abstract base class
// Members:
//   f  - the function itself
class Function {
public:
  virtual ~Function() {};
  virtual double f(const double& x) const =0;
}; // END Class Function

/**************************************************************************************/
// Abstract base class
// Members:
//   f  - the function itself
//   df - the function's first derivative
class FunctionAndDeriv{
public:
  virtual ~FunctionAndDeriv() {};
  virtual double f(const double& x) const =0;
  virtual double df(const double& x) const =0;
}; // END Class FunctionAndDeriv

/**************************************************************************************/
// Riemann discontinuity for initial condition of Riemann shock tube problem
class RiemannDiscontinuity : public Function {
public:
  // construct a step function with f(x) = { fL      x<x0
  //                                       { fR      else
  RiemannDiscontinuity(const double& fL, const double& fR, const double& x0):
    mfL(fL), mfR(fR), mx0(x0) {}
  ~RiemannDiscontinuity() {}
  double f(const double& x) const {
    if (x<mx0) return mfL;
    else return mfR;
  }
private:
  const double mfL, mfR, mx0;
}; // END Class RiemannDiscontinuity : Function

/**************************************************************************************/
// VelocityFunction
// fluid velocity to the right and left of a 1D shock
class VelocityFunction : public FunctionAndDeriv {
public:
  VelocityFunction(const double& rhoL, const double& PL, const double& vL,
                   const double& rhoR, const double& PR, const double& vR,
                   const double& Gamma):
    mrhoL(rhoL), mPL(PL), mvL(vL), mrhoR(rhoR), mPR(PR), mvR(vR), mGamma(Gamma),
    mcsL(sqrt(Gamma*PL/rhoL)), mAR(2/(Gamma+1)/rhoR), mBR((Gamma-1)*PR/(Gamma+1)) {}
  ~VelocityFunction() {}
  double getLSide(const double& Pstar) const {
    return mvL - 2*mcsL*(pow(Pstar/mPL,(mGamma-1)/2/mGamma)-1)/(mGamma-1);
  } 
  double getRSide(const double& Pstar) const {
    return mvR + (Pstar-mPR)*sqrt(mAR/(Pstar+mBR));
  } 
  double f(const double& Pstar) const {
    return getLSide(Pstar)-getRSide(Pstar);
  } 
  double df(const double& Pstar) const {
    double bigtermL = mcsL/mGamma*pow(mPL,(1-mGamma)/2/mGamma)*pow(Pstar,(-mGamma-1)/2/mGamma);
    double bigtermR1 = sqrt(mAR/(Pstar+mBR));
    double bigtermR2 = (Pstar-mPR)/2*sqrt(mAR/pow(Pstar+mBR,3));
    return -bigtermL - bigtermR1 + bigtermR2;
  }
private:
  const double mrhoL, mPL, mvL, mrhoR, mPR, mvR, mGamma, mcsL;
  const double mAR, mBR;
}; // END Class VelocityFunction : FunctionAndDeriv

#endif
