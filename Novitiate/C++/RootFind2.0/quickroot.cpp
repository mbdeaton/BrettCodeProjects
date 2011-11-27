// A root solver not requiring the first derivative of the function
//   Instructions: just add a Function Class for the function you're interested in solving
//   then change the parameters in main(): xmin,xmax,eps
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
};

// MassAction Class
//   From a Thermo assignment, from Callen section 13.2
class MassAction : public Function {
public:
	double f(double x) {

	  // from the example in the text (and problem 13.2-2)
	  return pow(x,1.5)/(2-x)/sqrt(2+x/2)/sqrt(2)-(0.0877)/sqrt(1e3);

	  // problem 13.2-7
	  // return 0.25*pow(0.71+2*x,2)*(0.63+x)/pow(x,2)/(1.34+x)-(0.0424)/(2e5);
	}
};

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
  Function * testF = new MassAction;

  // Bisection Method
  double eps = 0.0001;
  double xmin = 0;
  double xmax = 2.0;
  double f_at_min = testF->f(xmin);
  double f_at_max = testF->f(xmax);
  cout << " Bisection Method --------------------------" << endl;
  cout << "At x = " << xmin << ", the function is "
       << f_at_min << endl;
  cout << "At x = " << xmax << ", the function is "
       << f_at_max << endl;
  double root = RtBisect(testF,xmin,xmax,eps);
  cout << "The function has a root at x = " << root << endl;
  
  return 0;
}
