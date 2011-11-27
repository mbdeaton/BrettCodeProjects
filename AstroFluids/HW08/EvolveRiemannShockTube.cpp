// Evolution of the Riemann Shock Tube Problem
// Employs:
//   Timestepper: TVD-RK3
//   Courant Factor: 0.1
//   Riemann Solver: HLLE
//   Reconstructor: 1st order Godunov
//   Inverter: 1st order
//   Domain: x=[-0.5:0.5]
//   Discontinuity at x=0
//   Initial Conditions: (rho_L,P_L,v_L)=(1.0  , 1.0, 0)
//                       (rho_R,P_R,v_R)=(0.125, 0.1, 0)
// Brett Deaton - 11.2.11

#include"FDSolution.hpp"
#include"Function.hpp"
#include"DataWriter.hpp"
#include"AnalyticShockTubeSolution.hpp"

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include<sstream>
#include<string>

/**************************************************************************************/
void help(const std::string exec_name, const int num_expected_args) {
  std::cout << "Usage error. This executable requires " << num_expected_args
            << " arguments at the command line." << std::endl
            << "use: " << exec_name << " [method] res tdump filebasename" << std::endl
            << "  method:" << std::endl
	    << "                -rk3    = use 3rd order Runge-Kutta" << std::endl
	    << "                -rk3adv = use RK3 to evolve advection eq (for debug)" << std::endl
            << "  res:          int, number of grid points to lay down on x=[-0.5,0.5]" << std::endl
	    << "  tdump:        double, dump primitives at this evolution time" << std::endl
            << "  filename:     string, output filename (include extension, e.g. .dat)" << std::endl
            << "example: "<< exec_name << " -rk3 400 0.2 shock_tube-n400-t02.dat" << std::endl;
} // END Inline help

/**************************************************************************************/
int main(int argc, char **argv) {
  const int num_expected_args(4);
  if (argc!=num_expected_args+1) {
    help(argv[0],num_expected_args);
    return 1;
  }

  // set up integration boundaries
  const double xmin(-0.5), xmax(0.5);
  const double tmin(0), tmax(atof(argv[3]));
  const int nx(atoi(argv[2]));

  // initial configuration
  const int numvars(3);
  const double x0(0); // location of initial discontinuity
  const double rhoL0(1), PL0(1), vL0(0),
    rhoR0(0.125), PR0(0.1), vR0(0);
  const double Gamma(1.4);
  // prim vector is (rho, P, v)
  std::vector<double> primL0(3); primL0[0]=rhoL0; primL0[1]=PL0; primL0[2]=vL0;
  std::vector<double> primR0(3); primR0[0]=rhoR0; primR0[1]=PR0; primR0[2]=vR0;
  // set up dirichlet boundary condition p(x,t=tmin)=f(x) for p=(rho(x), P(x), v(x))
  Function* p0[numvars]; // array of Function pointers
  for (int j=0; j<numvars; ++j) { p0[j] = new RiemannDiscontinuity(primL0[j],primR0[j],x0);};

  // choose numerical scheme
  const std::string numerical_scheme(argv[1]);
  FDSolution* soln;
  if (numerical_scheme=="-rk3") {
    soln = new FullHydroRK3(p0,xmin,xmax,nx,tmin,tmax,Gamma);
  }
  else if (numerical_scheme=="-rk3adv") {
    const double charspeed(1);
    std::cout << "Debug: Solving advection equation for shock tube configuration with "
	      << "RK3, but using periodic boundary conditions. Warning: the last 3 columns "
	      << "in " << argv[4] << " are wrong in this case."  << std::endl;
    soln = new AdvectionRK3(p0,xmin,xmax,nx,tmin,tmax,Gamma,charspeed);
  }
  else {
    std::cout << "Error " << argv[0] << "::main: method option " << numerical_scheme
	      << " not recognized." << std::endl;
    help(argv[0],num_expected_args);
    return 1;
  }
  
  // create analytic solution for comparison
  AnalyticShockTubeSolution analytic_soln(rhoL0, PL0, vL0,
					  rhoR0, PR0, vR0,
					  Gamma);

  // Dump the solution
  const std::string file_name(argv[4]);
  const double t(soln->getFinalTime()); // this may be slightly larger than tmax
  std::stringstream header;
  header << "# Output from " << argv[0] << " " << numerical_scheme << " " << nx << " "
	 << tmax << " " << file_name << std::endl
	 << "# at time = " << t << std::endl
	 << "# nsteps  = " << soln->getNSteps() << std::endl
	 << "# [1] x" << std::endl
	 << "# [2] rho" << std::endl
	 << "# [3] P" << std::endl
	 << "# [4] v" << std::endl
	 << "# [5] rho (analytic)" << std::endl
	 << "# [6] P (analytic)" << std::endl
	 << "# [7] v (analytic)" << std::endl;
  OneDimDataWriter fout(file_name,header.str());
  for(int i=0; i<nx; ++i) {
    const double x(soln->getPosition(i));
    const double xovert(x/t);
    
    fout.AppendToFile(x,
		      soln->getDensity(i),
		      soln->getPressure(i),
		      soln->getVelocity(i),
		      analytic_soln.getDensity(xovert),
		      analytic_soln.getPressure(xovert),
		      analytic_soln.getVelocity(xovert));
  } // END for i (space loop)

  for (int j=0; j<numvars; ++j) delete p0[j]; // just a developing a habit
  delete soln;
  return 0;
} // END main
