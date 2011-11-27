// Brett Deaton 10.26.1l
// Function Class

#ifndef Function_H
#define Function_H

#include<math.h>

const double pi = 4.*atan(1);

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
// Square Wave with ability to specify height, and L & R bounds.
class SquareWave : public Function {
public:
  // construct a sqare pulse with f = { 0        x<xL | x>xR
  //                                  { height   else
  SquareWave(const double& height, const double& boundL, const double& boundR):
    mheight(height), mxL(boundL), mxR(boundR) {}
  ~SquareWave() {}
  double f(const double& x) const {
    if ((x<mxL) | (x>mxR)) return 0;
    else return mheight;
  }
private:
  const double mheight, mxL, mxR;
}; // END Class SquareWave : Function

/**************************************************************************************/
// Sine Wave with ability to specify wavelength, phase, and amplitude.
class SineWave : public Function {
public:
  // construct a sine wave amp*sin(2pi*x/lambda-phase)
  SineWave(const double& lambda, const double& phase, const double& amp):
    mlambda(lambda), mphase(phase), mamp(amp) {}
  ~SineWave() {}
  double f(const double& x) const {
    return mamp*sin(2*pi*x/mlambda-mphase);
  }
private:
  const double mlambda, mphase, mamp;
}; // END Class SineWave : Function

#endif
