// Employing Polymorphism: a root solver
//   Define a math-function class; pass a math-function object to root-finding
//   c-function which employs the Newton-Raphson method.
// Brett Deaton 7.09.10

#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<math.h>
#include<iostream>
#include<iomanip>
using namespace std;

// Constants
const double PI = 3.1415926535897932; // I guess pi isn't available in math.h

// Function Class
// Abstract base class
// Members:
//   f  - the function itself
//   df - the first derivative of the function
class Function {
public:  
  ~Function() {;}
  virtual double f(double x) =0;
  virtual double df(double x) =0;
};

// SinMinusX Class
class SinMinusX : public Function {
public:
  double f(double x) {
    return sin(PI*x)-x/2;
  }
  double df(double x) {
    return PI*cos(PI*x)-0.5;
  }
};

};

// Root Solve using the Newton-Raphson method
// Arguments:
//   myFunc   - pointer to a Function object
//   x        - first ordinal guess
//   eps      - epsilon: maximum error bound desired (on the x-axis)
// note: for an oscillating function, x must be chosen near enough
//   to the root of interest that the algorithm doesn't get confused
double RtSolve(Function *myFunc, double x, const double eps) {
  cout << "RtSolve is thinking ..." << endl;
  double rt = x - (myFunc->f(x)) / (myFunc->df(x));
  while (fabs(rt-x)>eps) {
    x = rt;
    rt = x - (myFunc->f(x)) / (myFunc->df(x));
    cout << x << endl;
  }
  return rt;
}

// Root Solve using the bisection method
// Arguments:
//   myFunc   - pointer to a Function object
//   xmin     - min ordinal value of the guess window
//   xmax     - max ordinal value of the guess window
//   eps      - epsilon: maximum error bound desired (on the x-axis)
// note: xmin and xmax must be chosen such that the function takes
//   on opposite signs at these two points
double RtBisect(Function *myFunc, double xmin, double xmax, const double eps) {
  cout << "RtBisect is thinking ..." << endl;
  cout.setf(ios::fixed); cout << setprecision(9); // output cleanup
  cout << "xmin" << "\t\t" << "xmid" << "\t\t" << "xmax" <<  endl;
  double xmid = (xmin+xmax)/2;
  while ((xmax-xmin)>eps) {
    if ((myFunc->f(xmid))*(myFunc->f(xmin))>=0) { xmin=xmid; }
    else { xmax=xmid; }
    cout << xmin << "\t" << xmid << "\t" << xmax << endl;
    xmid = (xmin+xmax)/2;
  }
  return xmid;
}


//********************************************************************//
//***************************** MAIN *********************************//
//********************************************************************//

int main() {
  Function * testF = new SinMinusX;

  // Newton-Raphson Method
  double x = 0.6;
  double f = testF->f(x);
  double df = testF->df(x);
  double eps = 1.e-6;
  cout << " Newton-Raphson Method ---------------------" << endl;
  cout << "At x = " << x << ", the function is " << f << "," << endl;
  cout << "and its derivative is " << df << endl;
  double root = RtSolve(testF,x,eps);
  cout << "The function has a root at x = " << root << endl << endl;
  
  // Bisection Method
  double xmin = 0.6;
  double xmax = 1.0;
  double f_at_min = testF->f(xmin);
  double f_at_max = testF->f(xmax);
  cout << " Bisection Method --------------------------" << endl;
  cout << "At x = " << xmin << ", the function is "
       << f_at_min << endl;
  cout << "At x = " << xmax << ", the function is "
       << f_at_max << endl;
  root = RtBisect(testF,xmin,xmax,eps);
  cout << "The function has a root at x = " << root << endl;

  return 0;
}
