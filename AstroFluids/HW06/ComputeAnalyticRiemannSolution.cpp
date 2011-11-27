// Analytic soln to the Riemann Shock Tube Problem from Astro Fluids 10.14.11
// Brett Deaton

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
      file_name        name of output file including extension
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
  const void AppendToFile(const double& x, const double& y, const double& z, const double& w) {
    mOut << x << "   " << y << "   " << z << "   " << w << std::endl;
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
  virtual double getRSide(const double& x) const =0;
  virtual double getLSide(const double& x) const =0;
  virtual double f(const double& x) const =0;
  virtual double df(const double& x) const =0;
}; // END Class Function

/**************************************************************************************/
// VelocityFunction
// fluid velocity to the right and left of a 1D shock
class VelocityFunction : public Function {
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
  const double mrhoL, mPL, mvL, mrhoR, mPR, mvR; // initial primities
  const double mGamma; // gas polytropic constant
  double mrhoLstar, mrhoRstar, mPstar, mvstar; // _star regions are to L & R of contact
  double mcsL, mcsR, mcsLstar, mcsRstar; // sound speeds
  double ms_s, ms_c, ms_t, ms_h; // discontinuity speeds (shock, contact, tail, head)
  void setPrimitives() {
    mcsL = sqrt(mGamma*mPL/mrhoL);
    mcsR = sqrt(mGamma*mPR/mrhoR);
    // compute contact discontinuity
    Function* velocityfunction = new VelocityFunction(mrhoL,mPL,mvL,mrhoR,mPR,mvR,mGamma);
    double Pstar_0 = 0.5*(mPR+mPL); // initial guess
    const double eps = 1.e-10*Pstar_0; // tolerance
    mPstar = RtSolve(velocityfunction,Pstar_0,eps);
    mvstar = velocityfunction->getLSide(mPstar);
    if (fabs(mvstar-velocityfunction->getRSide(mPstar))>eps)
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
}; // END Class AnalyticShockTubeSolution

/**************************************************************************************/
void help(const std::string exec_name, const int num_expected_args) {
  std::cout << "ERROR! This executable requires " << num_expected_args
            << " arguments at the command line." << std::endl
            << "use: " << exec_name << " <rhoL> <PL> <vL>"
	    << " <rhoR> <PR> <vR> <Gamma> <filename>" << std::endl
            << "  L params  = double, primitives to the left" << std::endl
            << "  R params  = double, primitives to the right" << std::endl
	    << "  Gamma     = double, polytropic index" << std::endl
            << "  filename  = str, output filename (include .dat extension)" << std::endl
            << "example: "<< exec_name << " 1 1 0 0.125 0.1 0 1.4 riemann.dat" << std::endl;
} // END Inline help

/**************************************************************************************/
int main(int argc, char **argv) {
  const int num_expected_args = 8;
  if (argc!=num_expected_args+1) {
    help(argv[0],num_expected_args);
    return 1;
  }

  const double rhoL(atof(argv[1])); const double PL(atof(argv[2])); const double vL(atof(argv[3]));
  const double rhoR(atof(argv[4])); const double PR(atof(argv[5])); const double vR(atof(argv[6]));
  const double Gamma(atof(argv[7]));
  const std::string file_name(argv[8]);
  std::stringstream header;
  header << "# Output from " << argv[0] << std::endl <<
    "# A plot of the similarity solution to the Riemann shock tube problem with primitives:" << std::endl << 
    "#   (rhoL,PL,vL) = (" << rhoL << "," << PL << "," << vL << ")" << std::endl <<
    "#   (rhoR,PR,vR) = (" << rhoR << "," << PR << "," << vR << ")" << std::endl <<
    "#   Polytropic Gamma = " << Gamma << std::endl <<
    "# [1] x/t" << std::endl << 
    "# [2] rho" << std::endl <<
    "# [3] P" << std::endl <<
    "# [4] v";
  OneDimDataWriter fout(file_name,header.str());

  AnalyticShockTubeSolution myShock(rhoL,PL,vL,rhoR,PR,vR,Gamma);
  const double lowerxovert(1.2*myShock.getFastestBackwardSpeed());
  const double upperxovert(1.2*myShock.getFastestForwardSpeed());
  const double N(500);
  const double dxovert((upperxovert-lowerxovert)/(N-1));
  double xovert;
  for (int i=0; i<N; ++i) {
    xovert = lowerxovert+i*dxovert;
    fout.AppendToFile(xovert,
		      myShock.getDensity(xovert),
		      myShock.getPressure(xovert),
		      myShock.getVelocity(xovert));
  }
  
  return 0;
} // END main
