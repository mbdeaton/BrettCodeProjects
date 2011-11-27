// Brett Deaton 10.25.11
// FDSolution Class
// Abstract base class
// Members:
//   u  - the function itself
//   u0 - the function at t=tmin
//   dt - the time step
//   dx - the spatial delta

#ifndef FDSolution_H
#define FDSolution_H

#include"Function.hpp"
#include<iostream>
#include<vector>

/**************************************************************************************/
// Abstract base class for finite difference integration using periodic bcs
class FDSolution {
public:
  // construct with a function u(t=tmin)
  FDSolution(const Function* u0, const double& xmin, const double& xmax, const int& nx,
	     const double& tmin, const double& tmax, const int& nt) :
    mu(nx,std::vector<double>(nt)), mNt(nt), mNx(nx), mxmin(xmin), mxmax(xmax),
    mtmin(tmin), mtmax(tmax),
    mdx((xmax-xmin)/nx), // because we enforce u(xmax)==u(xmin)
    mdt((tmax-tmin)/(nt-1)), notintegrated(true) {
    for(int i=0; i<mNx; ++i) mu[i][0]=u0->f(xmin+i*mdx);
    //     std::cout << "How big is the space dimension of u? " << mu.size() << std::endl;
    //     std::cout << "How big is the time dimension of u?  " << mu[0].size() << std::endl;
  };
  virtual ~FDSolution() {};
  void Integrateu() {
    if (notintegrated==true) IntegrateuImpl();
    notintegrated = false;
  };
  // getu(i,n) i space, n time index
  double getu(const int& i, const int& n) const {
    if(notintegrated) {
      std::cout << "Error FDSolution::getu: User must call Integrateu before calling getu."
		<< std::endl;
      return 0;
    }
    if(i<mNx && i>=0 && n<mNt && n>=0) return mu[i][n];
    else {
      std::cout << "Error FDSolution::getu: Requested value of u is outside t or x boundaries."
		<< std::endl;
      return 0;
    }
  }
protected:
  std::vector<std::vector<double> > mu; // mu[i][n]; i space, n time
  const int mNt, mNx;
  const double mxmin, mxmax, mtmin, mtmax, mdx, mdt;
  bool notintegrated;
private:
  // IntegrateuImpl is the FD implementation defined by the derived classes
  virtual void IntegrateuImpl() =0;
}; // END Class FDSolution

/**************************************************************************************/
// Implements the advection equation, integrated (unstably) with Forward Time Centered Space,
//   and periodic bcs
class AdvectionFTCS : public FDSolution {
public:
  AdvectionFTCS(const Function* u0, const double& xmin, const double& xmax, const int& nx,
		const double& tmin, const double& tmax, const int& nt, const double& charspeed) :
    FDSolution(u0, xmin, xmax, nx, tmin, tmax, nt), mcharspeed(charspeed) {};
  ~AdvectionFTCS() {};
private:
  void IntegrateuImpl() {
    for (int n=1; n<mNt; ++n) {
      for (int i=0; i<mNx; ++i) {
	if (i==0) {
	  mu[i][n] = mu[i][n-1]-mcharspeed*mdt/2/mdx*(mu[i+1][n-1]-mu[mNx-1][n-1]);
	}
	else if (i==(mNx-1)) {
	  mu[i][n] = mu[i][n-1]-mcharspeed*mdt/2/mdx*(mu[0][n-1]-mu[i-1][n-1]);
	}
	else {
	  mu[i][n] = mu[i][n-1]-mcharspeed*mdt/2/mdx*(mu[i+1][n-1]-mu[i-1][n-1]);
	}
      } // END for i space
    } // END for n time
  } // END IntegrateuImpl
  const double mcharspeed;
}; // END Class AdvectionFTCS : FDSolution

/**************************************************************************************/
// Implements the advection equation, integrated (stably) with Lax-Friedrichs,
//   and periodic bcs
class AdvectionLF : public FDSolution {
public:
  AdvectionLF(const Function* u0, const double& xmin, const double& xmax, const int& nx,
	      const double& tmin, const double& tmax, const int& nt, const double& charspeed) :
    FDSolution(u0, xmin, xmax, nx, tmin, tmax, nt), mcharspeed(charspeed) {};
  ~AdvectionLF() {};
private:
  void IntegrateuImpl() {
    for (int n=1; n<mNt; ++n) {
      for (int i=0; i<mNx; ++i) {
	if (i==0) {
	  mu[i][n] = (mu[i+1][n-1]+mu[mNx-1][n-1])/2
	    - mcharspeed*mdt/2/mdx*(mu[i+1][n-1]-mu[mNx-1][n-1]);
	}
	else if (i==(mNx-1)) {
	  mu[i][n] = (mu[0][n-1]+mu[i-1][n-1])/2
	    -mcharspeed*mdt/2/mdx*(mu[0][n-1]-mu[i-1][n-1]);
	}
	else {
	  mu[i][n] = (mu[i+1][n-1]+mu[i-1][n-1])/2
	    - mcharspeed*mdt/2/mdx*(mu[i+1][n-1]-mu[i-1][n-1]);
	}
      } // END for i space
    } // END for n time
  } // END IntegrateuImpl
  const double mcharspeed;
}; // END Class AdvectionLF : FDSolution

/**************************************************************************************/
// Implements the advection equation, integrated (stably) with Runge-Kutta 3rd Order,
//   and periodic bcs.
// This is a finite volume method, in which the time derivative depends on the fluxes
//   across cell faces. The fluxes are determined by Godunov's method (assuming a riemann
//   shock solution at the i+(1/2)th cell face, with u_L=u_i, u_R=u_{i+1}. Here I use
//   the approximate riemann solution defined by the HLLE scheme.

class AdvectionRK3 : public FDSolution {
public:
  AdvectionRK3(const Function* u0, const double& xmin, const double& xmax, const int& nx,
	       const double& tmin, const double& tmax, const int& nt, const double& charspeed) :
    FDSolution(u0, xmin, xmax, nx, tmin, tmax, nt), mcharspeed(charspeed) {};
  ~AdvectionRK3() {};
private:
  void IntegrateuImpl() {
    for (int n=1; n<mNt; ++n) {
      std::vector<double> uprev(mNt);
      for (int i=0; i<mNx; ++i) uprev[i]=mu[i][n-1]; // because I stupidly inverted my mu array
      std::vector<double> u1(mNx), u2(mNx);
      for (int i=0; i<mNx; ++i)	u1[i] = uprev[i] + mdt*ComputeRHS(uprev,i);
      for (int i=0; i<mNx; ++i)	u2[i] = 3*uprev[i]/4 + u1[i]/4 + mdt*ComputeRHS(u1,i)/4;
      for (int i=0; i<mNx; ++i)	mu[i][n] = uprev[i]/3 + 2*u2[i]/3 + 2*mdt*ComputeRHS(u2,i)/3;
    } // END for n time
  } // END IntegrateuImpl
  // compute a substep of the rk3 scheme, du/dt=RHS(u) given the previous substep u
  // (du/dt)_i = RHS = (f_{i-1/2}-f_{i+1/2})/dx
  double ComputeRHS(const std::vector<double>& u, const int& i) {
    if (i==0) {
      return (Flux(u[mNx-1],u[i])-Flux(u[i],u[i+1]))/mdx;
    }
    else if (i==(mNx-1)) {
      return (Flux(u[i-1],u[i])-Flux(u[i],u[0]))/mdx;
    }
    else {
      return (Flux(u[i-1],u[i])-Flux(u[i],u[i+1]))/mdx;
    }
  }
  // compute flux at the i+(1/2)th cell face, with u_L=u_i, u_R=u_{i+1}; uses
  //   Godunov's method, which for the advection equation is exactly
  //   f_{i+1/2} = a*{ u_i       a>0
  //                 { u_{i+1}   a<0
  //   No approximate Riemann solver need be employed
  double Flux(const double& ui, const double& uii) {
    if(mcharspeed>=0) return mcharspeed*ui;
    else return mcharspeed*uii;
  }
  const double mcharspeed;
}; // END Class AdvectionRK3 : FDSolution

#endif
