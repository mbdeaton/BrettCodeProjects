// Root finding with Newton-Raphson
// Brett Deaton 12.7.11

#ifndef FindRoot_H
#define FindRoot_H

#include"Function.hpp"
#include<iostream>
#include<math.h>

/**************************************************************************************/
// Root Solve using the Newton-Raphson method
// Arguments:
//   myFunc   - pointer to a FunctionAndDeriv object
//   x        - first ordinal guess
//   eps      - epsilon: maximum error bound desired (on the x-axis)
// note: not very robust
double RtSolve(const FunctionAndDeriv *myFunc, double x, const double eps) {
  double rt = x - (myFunc->f(x)) / (myFunc->df(x));
  int iter = 0, maxiter=100;
  bool debug = true;
  while (fabs(rt-x)>eps && iter<maxiter) {
    if (debug) std::cout << "  omega_" << iter << " = " << x << std::endl;
    x = rt;
    rt = x - (myFunc->f(x)) / (myFunc->df(x));
    ++iter;
  }
  if (iter==maxiter) {
    std::cout << "ERROR: reached max it; can't find root to desired accuracy." << std::endl;
  }
  return rt;
}; // END Inline RtSolve

#endif
