// Brett Deaton 11.7.11
// FDSolution Class
// Abstract base class
// Members:
//   u  - the conservative functions themselves
//   u0 - the functions at t=tmin
//   dx - the spatial delta

#ifndef FDSolution_H
#define FDSolution_H

#include"Function.hpp"
#include"HydroInverter.hpp"
#include"DataWriter.hpp"

#include<algorithm>
#include<iostream>
#include<math.h>
#include<vector>

/**************************************************************************************/
// Abstract base class for 1D finite element integration, of fluid primitives (rho,P,v)
//   Needs to know its integration boundaries and the number of grid points in x.
// tmax      FDSolution will integrate u to the first t for which t>=tmax (dt is adaptive)
// Gamma     adiabatic gas constant
class FDSolution {
public:
  // p0 primitives (rho,P,v) at the initial time slice
  FDSolution(Function** const p0, const double& xmin, const double& xmax,
	     const int& nx, const double& tmin, const double& tmax, const double& Gamma) :
    mu(3,std::vector<double>(nx)), mp(3,std::vector<double>(nx)),
    mNx(nx), mxmin(xmin), mxmax(xmax), mtmin(tmin), mtmax(tmax), mdx((xmax-xmin)/(nx-1)),
    mGamma(Gamma), mtfinal(tmin), nsteps(0), mnumvars(3), EPS(1e-6), notintegrated(true) {
    for(int j=0; j<mnumvars; ++j) {
      for(int i=0; i<mNx; ++i) {
	mp[j][i]=p0[j]->f(xmin+i*mdx);
      }
    }
    setConsFromPrim();
  };
  virtual ~FDSolution() {};
  // get final conservatives at one spatial index, i
  // returns u(x_i,t_final)=(rho,S,E)
  std::vector<double> getConservatives(const int& i) {
    Integrateu();
    std::vector<double> cons(3);
    if(i<mNx && i>=0) {
      for (int j=0; j<mnumvars; ++j) cons[j]=mu[j][i];
    }
    else {
      std::cout << "Error FDSolution::getConservatives: Requested value of u "
		<< "is outside x boundaries." << std::endl;
    }
    return cons;
  };
  // get final primitives at one spatial index, i
  double getDensity(const int& i) {Integrateu(); return mp[0][i];};
  double getPressure(const int& i) {Integrateu(); return mp[1][i];};
  double getVelocity(const int& i) {Integrateu(); return mp[2][i];};
  double getFinalTime() {Integrateu(); return mtfinal;};
  int getNSteps() {Integrateu(); return nsteps;}
  double getPosition(const int& i) const {
    double x(mxmin+i*mdx);
    if((i>=mNx) | (i<0)) {
      std::cout << "Error FDSolution::getPosition: Requested value of x "
		<< "is outside x boundaries." << std::endl;
    }
    return x;
  };
protected:
  void Integrateu() {
    if (notintegrated==true) IntegrateuImpl();
    InvertConsToPrim();
    notintegrated = false;
  };
  std::vector<std::vector<double> > mu; // mu[j][i]; jth variable, ith position
  //                                       mu[0]=rho; mu[1]=Sflux; mu[2]=E
  std::vector<std::vector<double> > mp; // mp[0]=rho; mp[1]=Press; mp[2]=v                  
  const int mNx;
  const double mxmin, mxmax, mtmin, mtmax, mdx;
  const double mGamma;
  double mtfinal; // because of variable stepsize tfinal!=tmax (at least not necessarily)
  int nsteps; // keep track of total number of steps taken
  const int mnumvars;
  const double EPS; // machine eps to protect from div by zero
  bool notintegrated;
private:
  // called once at beginning to set conservatives from primitives
  void setConsFromPrim() {
    for(int i=0; i<mNx; ++i) {
      double rho(mp[0][i]), P(mp[1][i]), v(mp[2][i]);
      mu[0][i]=rho;
      mu[1][i]=rho*v;
      mu[2][i]=rho*v*v/2+P/(mGamma-1);
    }
  };
  // InvertConsToPrim is the inversion implementation defined by the derived classes
  virtual void InvertConsToPrim() =0;
  // IntegrateuImpl is the FD implementation defined by the derived classes
  virtual void IntegrateuImpl() =0;
}; // END Class FDSolution

/**************************************************************************************/
// Implements the advection equation in 3 variables (kind of silly, but useful for
//   debugging, because it's easy), integrated with Runge-Kutta 3rd order scheme.
//   Uses periodic boundary conditions.
// This is a finite volume method, in which the time derivative depends on the fluxes
//   across cell faces. The fluxes are determined by Godunov's method (assuming a riemann
//   shock solution at the i+(1/2)th cell face, with u_L=u_i, u_R=u_{i+1}. Here I use
//   the approximate riemann solution defined by the HLLE scheme.
class AdvectionRK3 : public FDSolution {
public:
  // p0 are the primitives (rho,P,v) at the initial time slice
  AdvectionRK3(Function** const p0, const double& xmin, const double& xmax,
	       const int& nx, const double& tmin, const double& tmax,
	       const double& Gamma, const double& charspeed) :
    FDSolution(p0, xmin, xmax, nx, tmin, tmax, Gamma),
    mcharspeed(charspeed), mcourant_factor(0.1), mdt(mdx*mcourant_factor/mcharspeed),
    mhydroinverter(Gamma) {
  };
  ~AdvectionRK3() {};
private:
  void InvertConsToPrim() {
    mp = mhydroinverter.getPrimitives(mu); // first order piecewise constant inversion
  };
  void IntegrateuImpl() {
    while(mtfinal<mtmax) {    
      for (int j=0; j<mnumvars; ++j) {
	std::vector<double> uprev=mu[j];
	std::vector<double> u1(mNx), u2(mNx);
	for (int i=0; i<mNx; ++i) u1[i] = uprev[i] + mdt*ComputeRHS(uprev,i);
	for (int i=0; i<mNx; ++i) u2[i] = 3*uprev[i]/4 + u1[i]/4 + mdt*ComputeRHS(u1,i)/4;
	for (int i=0; i<mNx; ++i) mu[j][i] = uprev[i]/3 + 2*u2[i]/3 + 2*mdt*ComputeRHS(u2,i)/3;
      } // END for j variable
      mtfinal+=mdt; ++nsteps;
    } // END while time
  } // END IntegrateuImpl
  // compute a substep of the rk3 scheme, du/dt=RHS(u) given the previous substep u
  // (du/dt)_i = RHS = (f_{i-1/2}-f_{i+1/2})/dx
  double ComputeRHS(const std::vector<double>& u, const int& i) const {
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
  double Flux(const double& ui, const double& uii) const {
    if(mcharspeed>=0) return mcharspeed*ui;
    else return mcharspeed*uii;
  }
  const double mcharspeed;
  const double mcourant_factor;
  const double mdt; // constant in this scheme
  HydroInverter mhydroinverter;
}; // END Class AdvectionRK3 : FDSolution

/**************************************************************************************/
// Implements the Full Hydro equations, integrated with Runge-Kutta TVD 3rd Order.
//   I'm imposing some sort of bound cond (neumann?) by forcing flux=0 at the boundaries.
// This is a finite volume method, in which the time derivative depends on the fluxes
//   across cell faces. The fluxes are determined by Godunov's method (assuming a riemann
//   shock solution at the i+(1/2)th cell face, with u_L=u_i, u_R=u_{i+1}. Here I use
//   the approximate riemann solution defined by the HLLE scheme.
// u[0] = rho
// u[1] = Sflux = rho*v
// u[2] = E = rho*v*v/2 + U
class FullHydroRK3 : public FDSolution {
public:
  // p0 are the primitives (rho,P,v) at the initial time slice
  FullHydroRK3(Function** const p0, const double& xmin, const double& xmax,
               const int& nx, const double& tmin, const double& tmax,
	       const double& Gamma) :
    FDSolution(p0, xmin, xmax, nx, tmin, tmax, Gamma),
    fluxes(mnumvars,std::vector<double>(mNx+1)), speeds(2,std::vector<double>(mNx)),
    mcourant_factor(0.1),
    debug_minstep(0), debug_maxstep(1000),
    debug_subroutine_order(false), debug_stepsize(false), debug_fluxes(false),
    debug_vars(false), debug_soundspeed(false),
    // dout_fluxes("debug_flux.yg","# Debugging output from FullHydroRK3, fluxes"),
    // dout_vars("debug_vars.yg","# Debugging output from FullHydroRK3, vars"),
    mhydroinverter(Gamma), speeds_are_not_set(true),
    prims_are_not_set(false), // prims are set by the base class constructor
    fluxes_are_not_set(true), dt_is_not_set(true) {
    setSpeeds(); setTimeStep(); setFluxes();
    checkVars();
  };
  ~FullHydroRK3() {};
private:
  void InvertConsToPrim() {
    if (prims_are_not_set) {
      if (debug_subroutine_order && (nsteps>=debug_minstep) && (nsteps<=debug_maxstep))
	std::cout << "executing InvertConsToPrim" << std::endl;
      mp = mhydroinverter.getPrimitives(mu); // first order piecewise constant inversion
      prims_are_not_set = false;
    } // END if prims_are_not_set
  };

  // 3rd order Total Variation Diminishing Runge-Kutta algorithm
  void IntegrateuImpl() {
    while (mtfinal<mtmax) {
      if ((nsteps>=debug_minstep) && (nsteps<=debug_maxstep))
	std::cout << "--- Time Step n=" << nsteps << ", t=" << mtfinal << " ---" << std::endl;

      // first substep
      const std::vector<std::vector<double> > uprev(mu);
      for (int i=0; i<mNx; ++i) {
	for (int j=0; j<mnumvars; ++j) {
	  mu[j][i] = uprev[j][i] + dt*ComputeRHS(j,i);
	}
      }
      speeds_are_not_set=true; prims_are_not_set=true; fluxes_are_not_set=true;

      // second substep
      const std::vector<std::vector<double> > u1(mu);
      InvertConsToPrim(); setSpeeds(); setFluxes();
      checkVars();
      for (int i=0; i<mNx; ++i) {
	for (int j=0; j<mnumvars; ++j) {
	  mu[j][i] = 3*uprev[j][i]/4 + u1[j][i]/4 + dt*ComputeRHS(j,i)/4;
	}
      }
      speeds_are_not_set=true; prims_are_not_set=true; fluxes_are_not_set=true;

      // third (final) substep
      const std::vector<std::vector<double> > u2(mu);
      InvertConsToPrim(); setSpeeds(); setFluxes();
      checkVars();
      for (int i=0; i<mNx; ++i) {
	for (int j=0; j<mnumvars; ++j) {
	  mu[j][i] = uprev[j][i]/3 + 2*u2[j][i]/3 + 2*dt*ComputeRHS(j,i)/3;
	}
      }
      mtfinal+=dt; ++nsteps;
      speeds_are_not_set=true; prims_are_not_set=true; fluxes_are_not_set=true; dt_is_not_set=true;
      InvertConsToPrim(); setSpeeds(); setFluxes();
      checkVars();
    } // END while time
    std::cout << "Evolving hydro equations with with RK3." << std::endl;
    std::cout << "final time = " << mtfinal << std::endl;
    std::cout << "nsteps = " << nsteps << std::endl;
  };

  // compute a substep of the rk3 scheme, du/dt=RHS(u). For j-th variable at i-th position,
  // returns (f_{i-1/2}-f_{i+1/2})/dx where the f are the fluxes on the cell faces.
  // fluxes vector is shifted left from the grid , so that
  // flux_i = fluxL(u_i) = fluxR(u_{i-1}) and
  // flux_{i+1} = fluxL(u_{i+1}) = fluxR(u_i)
  double ComputeRHS(const int& j, const int& i) {
    setFluxes();
    const double FL(fluxes[j][i]), FR(fluxes[j][i+1]);
    return (FL-FR)/mdx;
  };

  void setTimeStep() {
    if(dt_is_not_set) {
      if (debug_subroutine_order && (nsteps>=debug_minstep) && (nsteps<=debug_maxstep))
	std::cout << "executing setTimeStep" << std::endl;
      InvertConsToPrim();
      setSpeeds();
      dt=mdx*mcourant_factor/(fastestspeed+EPS);
      if (debug_stepsize && (nsteps>=debug_minstep) && (nsteps<=debug_maxstep))
	std::cout << "  dt = " << dt << std::endl;
      dt_is_not_set = false;
    }
  };

  // to compute flux at the i-(1/2)th cell face, with u_L=u_{i-1}, u_R=u_i, use
  //   Godunov's method, HLLE approximation:
  //   f_{i-1/2} = { FL       Sminus>=0
  //               { Fstar    Sminus<0<Splus
  //               { FR       Sminus<=0
  //   where the L flux is F(u_L), R flux is F(u_R), and Fstar is complicated, look it up,
  //   and speed Sminus (Splus) is fastest left-going (right-going) speed in both bordering cells
  void setFluxes() {
    InvertConsToPrim();
    setSpeeds();
    if (fluxes_are_not_set) {
      if (debug_subroutine_order && (nsteps>=debug_minstep) && (nsteps<=debug_maxstep))
	std::cout << "executing setFluxes" << std::endl;
      // we keep fluxes[j][0]=fluxes[j][mNx]=fluxes_j(t=tmin)|_boundaries
      if (nsteps==0) {
	// set flux primitives for far left and far right boundaries
	const double
	  rhofarL(mp[0][0]), rhofarR(mp[0][mNx-1]),
	  PfarL(mp[1][0]), PfarR(mp[1][mNx-1]),
	  vfarL(mp[2][0]), vfarR(mp[2][mNx-1]),
	  EfarL(mu[2][0]), EfarR(mu[2][mNx-1]);
	fluxes[0][0]=rhofarL*vfarL;
	fluxes[0][mNx]=rhofarR*vfarR;
	fluxes[1][0]=rhofarL*vfarL*vfarL+PfarL;
	fluxes[1][mNx]=rhofarR*vfarR*vfarR+PfarR;
	fluxes[2][0]=vfarL*(EfarL+PfarL);
	fluxes[2][mNx]=vfarR*(EfarR+PfarR);
      }
      // there are mNx+1 cell faces
      for (int i=1; i<mNx; ++i) {
	const double Sminus(std::min(speeds[0][i-1],speeds[0][i]));
	const double Splus(std::max(speeds[1][i-1],speeds[1][i]));
	if (Splus<Sminus) {
	  std::cout << "Error FullHydro::setFluxes: Splus < Sminus" << std::endl;
	}
	const double
	  rhoL(mp[0][i-1]), rhoR(mp[0][i]),
	  PL(mp[1][i-1]), PR(mp[1][i]),
	  vL(mp[2][i-1]), vR(mp[2][i]),
	  EL(mu[2][i-1]), ER(mu[2][i]);
	std::vector<double> FL(mnumvars), FR(mnumvars);
	FL[0]=rhoL*vL;       FR[0]=rhoR*vR;
	FL[1]=rhoL*vL*vL+PL; FR[1]=rhoR*vR*vR+PR;
	FL[2]=vL*(EL+PL);    FR[2]=vR*(ER+PR);
	if (Sminus>0) for(int j=0; j<mnumvars; ++j) fluxes[j][i]=FL[j];
	else if (Splus<0) for(int j=0; j<mnumvars; ++j) fluxes[j][i]=FR[j];
	else
	  for(int j=0; j<mnumvars; ++j) {
	    fluxes[j][i]=
	      (Splus*FL[j]-Sminus*FR[j]+Sminus*Splus*(mu[j][i]-mu[j][i-1]))
	      / (Splus-Sminus+EPS); // this is Fstar
	  }
      } // END for i space
      fluxes_are_not_set = false;
      // if (debug_fluxes && (nsteps>=debug_minstep) && (nsteps<=debug_maxstep)) {
      //   std::vector<double> x_faces(mNx+1);
      //   for (int i=0; i<(mNx+1); ++i) x_faces[i]=mxmin+((double)i-0.5)*mdx;
      //   dout_fluxes.AppendToFile(mtfinal,x_faces,fluxes[2]);
      // }
    } // END if fluxes_are_not_set
  };

  // use primitives mp to calculate (v-cs,v+cs) for each cell, also set fastestspeed
  void setSpeeds() {
    InvertConsToPrim();
    if (speeds_are_not_set) {
      if (debug_subroutine_order && (nsteps>=debug_minstep) && (nsteps<=debug_maxstep))
	std::cout << "executing setSpeeds" << std::endl;
      double x_of_fastestspeed(0);
      fastestspeed=0;
      for (int i=0; i<mNx; ++i) {
	const double v(mp[2][i]), cs(sqrt(mGamma*mp[1][i]/(mp[0][i]+EPS)));
	if (debug_soundspeed && cs!=cs)
	  std::cout << "NAN in soundspeed" << std::endl
		    << "  step = " << nsteps << std::endl
		    << "  time = " << mtfinal << std::endl
		    << "  rho  = " << mp[0][i] << std::endl
		    << "  P    = " << mp[1][i] << std::endl
		    << "  v    = " << mp[2][i] << std::endl
		    << "  x    = " << mxmin+i*mdx << std::endl;
	speeds[0][i]=v-cs;
	if (fastestspeed<=abs(speeds[0][i])) {
	  fastestspeed = abs(speeds[0][i]);
	  x_of_fastestspeed=mxmin+i*mdx;
	}
	speeds[1][i]=v+cs;
	if (fastestspeed<=abs(speeds[1][i])) {
	  fastestspeed = abs(speeds[1][i]);
	  x_of_fastestspeed=mxmin+i*mdx;
	}
	if (debug_soundspeed && ((speeds[0][i]>0) | (speeds[1][i]<0)))
	  std::cout << "fluid velocity is supersonic" << std::endl
		    << "  step = " << nsteps << std::endl
		    << "  x    = " << mxmin+(double)i*mdx << std::endl
		    << "  v    = " << v << std::endl
		    << "  cs   = " << cs << std::endl
		    << "  s-   = " << speeds[0][i] << std::endl
		    << "  s+   = " << speeds[1][i] << std::endl;
      }
      if (debug_stepsize && (nsteps>=debug_minstep) && (nsteps<=debug_maxstep))
	std::cout << "  fastestspeed = " << fastestspeed << std::endl
		  << "  location of fastestspeed = " << x_of_fastestspeed << std::endl;
      speeds_are_not_set = false;
    } // END if speeds_are_not_set
  };

  // for debugging
  void checkVars() {
    if (debug_vars && (nsteps>=debug_minstep) && (nsteps<=debug_maxstep)) {
      std::vector<double> x(mNx);
      for (int i=0; i<mNx; ++i) {
	if ((mu[0][i]!=mu[0][i]) | (mu[1][i]!=mu[1][i]) | (mu[2][i]!=mu[2][i]) |
	    (mp[0][i]!=mp[0][i]) | (mp[1][i]!=mp[1][i]) | (mp[2][i]!=mp[2][i])) {
	  std::cout << "NAN in some variable" << std::endl
		    << "step = " << nsteps << std::endl
		    << "x = " << mxmin+i*mdx << std::endl
		    << "  rho(prim) = " << mp[0][i] << std::endl
		    << "  P         = " << mp[1][i] << std::endl
		    << "  v         = " << mp[2][i] << std::endl
		    << "  rho(cons) = " << mu[0][i] << std::endl
		    << "  Sflux     = " << mu[1][i] << std::endl
		    << "  E         = " << mu[2][i] << std::endl
		    << "  cs^2      = " << mGamma*mp[1][i]/mp[0][i] << std::endl;
	}
	x[i]=mxmin+i*mdx;
      }
      // dout_vars.AppendToFile(mtfinal,x,mp[2]);
    }
  };
  
  double fastestspeed;
  std::vector<std::vector<double> > fluxes; // f[i] = f_{i-1/2} = fluxL(u_i) = fluxR(u{i-1})
  std::vector<std::vector<double> > speeds; // - and + speeds (v-cs,v+cs) in each cell
  //                                           speeds[0][i] is minus speed in ith cell
  const double mcourant_factor;
  const int debug_minstep, debug_maxstep; // master debug flags: prints only for steps min<=n<=max
  const bool debug_subroutine_order; // true for prints related to order of execution
  const bool debug_stepsize; // true for prints related to stepsize
  const bool debug_fluxes; // true for fluxes dumped to YgraphDataWriter dout
  const bool debug_vars; // true for variables dumped to YgraphDataWriter dout
  const bool debug_soundspeed; // true for prints related to soundspeed
  // YgraphDataWriter dout_fluxes, dout_vars; // for debugging with ygraph animations
  double dt; // adaptive in this scheme
  HydroInverter mhydroinverter;
  bool speeds_are_not_set, prims_are_not_set, fluxes_are_not_set,
    dt_is_not_set; // these are true respectively whenever
  //                  speeds, primitives, fluxes, or time step size
  //                  do not correspond to the mu on hand.
}; // END Class FullHydroRK3 : FDSolution

#endif
