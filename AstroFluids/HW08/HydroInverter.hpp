// Brett Deaton 11.8.11
// HydroInverter Class inverts (rho,Sflux,E) --> (rho,P,v)
// Abstract base class to come, for now, I'm just coding one method
// Members:
//   u  - the conservative functions
//   p  - the primitive functions

#ifndef HydroInverter_H
#define HydroInverter_H

#include<iostream>
#include<vector>

/**************************************************************************************/
// 1D hydro inverter, assuming piecewise constant u; and ideal gas eos.
// Use: feed getPrimitives a conservative vector<vector<double> > (of size 3 in first
//      index, of any size in second index, x), and get back a vector<vector<double> >
//      (same size) of primitives.
class HydroInverter {
public:
  HydroInverter(const double& Gamma) :
    mGamma(Gamma), mNx(1), numvars(3), EPS(1e-16), debug_vars(false),
    mu(3,std::vector<double>(mNx)), mp(3,std::vector<double>(mNx)) {};
  ~HydroInverter() {};
  // argument cons, u[i] = (rho[i],         S[i],          E[i]          )
  //                       (matter density, momentum flux, energy density)
  // returns prim, p[i] = (rho[i],         P[i],     v[i]    )
  //                      (matter density, pressure, velocity)
  std::vector<std::vector<double> > getPrimitives(const std::vector<std::vector<double> >& cons) {
    mNx=(int)cons[0].size();
    mu=cons;
    mp=mu; // just for resizing purposes
    setPrimitives();
    return mp;
  };
protected:
  void setPrimitives() {
    for (int j=0; j<numvars; ++j) {
      if ((mu[j].size()!=mp[j].size()) | ((int)mu[j].size()!=mNx))
	std::cout << "Error HydroInverter::setPrimitives: size mismatch between cons & prim."
		  << std::endl;
    }
    for(int i=0; i<mNx; ++i) {
      const double rho(mu[0][i]), S(mu[1][i]), E(mu[2][i]);
      const double v(S/(rho+EPS));
      const double P((mGamma-1)*(E-rho*v*v/2));
      mp[0][i]=rho; mp[1][i]=P; mp[2][i]=v;
      if (debug_vars && ((rho!=rho) | (S!=S) | (E!=E) | (v!=v) | (P!=P))) {
	std::cout << "NAN in some variable" << std::endl
		  << "  rho  = " << rho << std::endl
		  << "  P    = " << P << std::endl
		  << "  v    = " << v << std::endl
		  << "  Sflux= " << S << std::endl
		  << "  E    = " << E << std::endl;
      }
    }
  };
  const double mGamma; // adiabatic constant
  int mNx; // number of points in x
  const int numvars;
  const double EPS; // machine eps to protect from div by zero
  const bool debug_vars; // true for print statements for debugging variables
  std::vector<std::vector<double> > mu; // hydro conservatives, u = (rho, Sflux, E)
  //                                  with Sflux=rho*v, E=rho*v*v/2+P
  std::vector<std::vector<double> > mp; // hydro primitives, p = (rho,P,v)
}; // END Class HydroInverter

#endif
