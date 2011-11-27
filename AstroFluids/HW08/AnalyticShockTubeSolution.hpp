// Analytic soln to the Riemann Shock Tube Problem from Astro Fluids 10.14.11
// Brett Deaton

#include"Function.hpp"

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<fstream>
#include<iostream>
#include<sstream>
#include<iomanip>

/**************************************************************************************/
// Compute 1D shock tube state, with access to the primitive variables as fxns of x/t
// WARNING: Assumes from L->R: rarefaction head, tail, contact discontinuity, shock.
//   More fundamentally, these assumptions are rhoL>rhoR, PL>PR, and (vR-vL) small.
class AnalyticShockTubeSolution {
public:
  /*Constructor:
    Set up the state with the primitives (rho,P,v) known on the L & R
      rhoL, PL, vL     conservatives at x<0 at t=0
      rhoR, PR, vR     conservatives at x>0 at t=0
      Gamma            gas polytropic index
    ToDo: set up some require statements about the assumptions
  */
  AnalyticShockTubeSolution(const double& rhoL, const double& PL, const double& vL,
			    const double& rhoR, const double& PR, const double& vR, const double& Gamma):
    mrhoL(rhoL), mPL(PL), mvL(vL), mrhoR(rhoR), mPR(PR), mvR(vR), mGamma(Gamma) {setPrimitives();};
  ~AnalyticShockTubeSolution() {};
  double getDensity(const double& xovert) const {
    if(xovert<ms_s) {
      if(xovert<ms_c) {
	if(xovert<ms_t) {
	  if(xovert<ms_h) {
	    return mrhoL; // to the left of rarefaction
	  }
	  else return mrhoL*pow(getPressure(xovert)/mPL,1./mGamma); // in rarefaction fan
	}
	else return mrhoLstar; // between contact and tail
      }
      else return mrhoRstar; // between shock & contact
    }
    else return mrhoR; // to the right of shock
  };
  double getPressure(const double& xovert) const {
    if(xovert<ms_s) {
      if(xovert<ms_t) {
	if(xovert<ms_h) {
	  return mPL; // to the left of rarefaction
	}
	else return mPL*pow(2/(mGamma+1)+(mGamma-1)*(mvL-xovert)/(mGamma+1)/mcsL,
			    2*mGamma/(mGamma-1)); // in rarefaction fan
      }
      else return mPstar; // between shock and tail
    }
    else return mPR; // to the right of shock
  };
  double getVelocity(const double& xovert) const {
    if(xovert<ms_s) {
      if(xovert<ms_t) {
	if(xovert<ms_h) {
	  return mvL; // to the left of rarefaction
	}
	else return (mcsL+(mGamma-1)*mvL/2+xovert)*2/(mGamma+1); // in rarefaction fan
	// else return xovert-sqrt(mGamma*getPressure(xovert)/getDensity(xovert)); // in rarefaction fan
      }
      else return mvstar; // between shock and tail
    }
    else return mvR; // to the right of shock
  };
  double getFastestForwardSpeed() const {return ms_s;};
  double getFastestBackwardSpeed() const {return ms_h;};
private:
  void setPrimitives() {
    mcsL = sqrt(mGamma*mPL/mrhoL);
    mcsR = sqrt(mGamma*mPR/mrhoR);
    // compute contact discontinuity
    FunctionAndDeriv* function = new VelocityFunction(mrhoL,mPL,mvL,
						      mrhoR,mPR,mvR,
						      mGamma); // for root solve
    VelocityFunction vfunction(mrhoL,mPL,mvL,
			       mrhoR,mPR,mvR,
			       mGamma); // for access to L & R side
    double Pstar_0 = 0.5*(mPR+mPL); // initial guess
    const double eps = 1.e-10*Pstar_0; // tolerance
    mPstar = RtSolve(function,Pstar_0,eps);
    mvstar = vfunction.getLSide(mPstar);
    if (fabs(mvstar-vfunction.getRSide(mPstar))>eps)
      std::cout << "ERROR: Root find failed" << std::endl; // vLstar == vRstar
    ms_c = mvstar; // contact discontinuity speed
    // compute shock
    mrhoRstar = mrhoR*(mPstar/mPR+(mGamma-1)/(mGamma+1))/((mGamma-1)*mPstar/(mGamma+1)/mPR+1);
    ms_s = mvR + mcsR*sqrt((mGamma+1)*mPstar/mPR/2/mGamma+(mGamma-1)/2/mGamma); // shock speed
    mcsRstar = sqrt(mGamma*mPstar/mrhoRstar); // sound speed to R of contact
    // compute rarefaction
    mrhoLstar = mrhoL*pow(mPstar/mPL,1./mGamma);
    mcsLstar = sqrt(mGamma*mPstar/mrhoLstar); // sound speed to L of contact
    ms_h = mvL-mcsL; // rarefaction head speed
    ms_t = mvstar-mcsLstar; // rarefaction tail speed
  };
  // Root Solve using the Newton-Raphson method
  // Arguments:
  //   myFunc   - pointer to a Function object
  //   x        - first ordinal guess
  //   eps      - epsilon: maximum error bound desired (on the x-axis)
  // note: not very robust
  double RtSolve(const FunctionAndDeriv* myFunc, double x, const double eps) const {
    double rt = x - (myFunc->f(x)) / (myFunc->df(x));
    int iter = 0, maxiter=100000;
    bool debug = false;
    while (fabs(rt-x)>eps && iter<maxiter) {
      if (debug) std::cout << "  Pstar_" << iter << " = " << x << std::endl;
      x = rt;
      rt = x - (myFunc->f(x)) / (myFunc->df(x));
      // std::cout << x << endl;
      ++iter;
    }
    if (iter==maxiter) {
      std::cout << "ERROR: RtSolve reached max iterations; "
		<< "unable to find root within desired accuracy." << std::endl;
    }
    return rt;
  }; // END RtSolve
  const double mrhoL, mPL, mvL, mrhoR, mPR, mvR; // initial primities
  const double mGamma; // gas polytropic constant
  double mrhoLstar, mrhoRstar, mPstar, mvstar; // _star regions are to L & R of contact
  double mcsL, mcsR, mcsLstar, mcsRstar; // sound speeds
  double ms_s, ms_c, ms_t, ms_h; // discontinuity speeds (shock, contact, tail, head)
}; // END Class AnalyticShockTubeSolution

