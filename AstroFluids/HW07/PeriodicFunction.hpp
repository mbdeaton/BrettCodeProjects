// Brett Deaton 10.27.1l
// Periodic Function Class
// WARNING: Completely Unfinished

#ifndef PeriodicFunction_H
#define PeriodicFunction_H

#include<math.h>

const double pi = 4.*atan(1);

/**************************************************************************************/
// Abstract base class
// Members:
//   f  - the function itself
class Function {
public:
  virtual ~Function() =0;
  virtual double f(const double& x) const =0;
}; // END Class Function

/**************************************************************************************/
// Abstract derived class
// Members:
//   f  - the function
//   fImpl - implementation of that function in the derived classes
class PeriodicFunction : public Function {
public:
  PeriodicFunction(const double& xmin, const double& xmax):
    mxmin(xmin), mxmax(xmax), mxrange(xmax-xmin) {};
  virtual ~PeriodicFunction() {};
  virtual double f(const double& x) const {
    int nwraps(x);
    double mx = mod();
    fImpl(mx)
  };
private:
  virtual double fImpl(const double& x) const =0;
  const double mxmin, mxmax, mxrange;
}; // END Class PeriodicFunction

/**************************************************************************************/
// Square Wave with ability to specify height, and L & R bounds.
class SquareWave : public PeriodicFunction {
public:
  // construct a sqare pulse with f = { 0        x<xL | x>xR
  //                                  { height   else
  SquareWave(const double& height, const double& boundL, const double& boundR):
    mheight(height), mxL(boundL), mxR(boundR) {};
  ~SquareWave() {};
  double f(const double& x) const {
    if ((x<mxL) | (x>mxR)) return 0;
    else return mheight;
  };
private:
  const double mheight, mxL, mxR;
}; // END Class SquareWave : PeriodicFunction

/**************************************************************************************/
// Sine Wave with ability to specify wavelength, phase, and amplitude.
class SineWave : public PeriodicFunction {
public:
  // construct a sine wave amp*sin(2pi*x/lambda-phase)
  SineWave(const double& lambda, const double& phase, const double& amp):
    mlambda(lambda), mphase(phase), mamp(amp) {};
  ~SineWave() {};
  double f(const double& x) const {
    return mamp*sin(2*pi*x/mlambda-mphase);
  };
private:
  const double mlambda, mphase, mamp;
}; // END Class SineWave : PeriodicFunction

#endif
