// Numerical evolution of the Advection Equation from Astro Fluids
//   Evolves a function u, subject to d_t{u} + a d_x{u} = 0, with periodic boundary
//   conditions at x=[-0.5,0.5], and characteristic speed a=1. Evolution proceeds
//   on t=[0,tmax]. User specifies u at t=0 (from precoded options), numerical scheme,
//   grid resolution, final time, and number of data dumps to take in time. Each data
//   dump makes a new output file with the time index (0,1,2...ndump-1) as a tag. The
//   time step is chosen such that the courant factor is 0.5. Of course, the analytic
//   solution given u0(x)=u(x,t=0) is u0(x-at)
// Brett Deaton - 10.25.11

#include"FDSolution.hpp"
#include"Function.hpp"
#include"OneDimDataWriter.hpp"

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
            << "use: " << exec_name << " [wavetype] [method] nxres ntdump filebasename" << std::endl
            << "  wavetype:" << std::endl
	    << "                -sine   = advect a sine wave" << std::endl
            << "                -square = advect a square wave" << std::endl
            << "  method:" << std::endl
	    << "                -ftcs   = use forward time, centered space" << std::endl
	    << "                -lf     = use Lax-Friedrichs scheme" << std::endl
	    << "                -rk3    = use 3rd order TVD Runge-Kutta" << std::endl
            << "  nxres:        int, number of grid points to lay down on x=[-0.5,0.5]" << std::endl
	    << "  tfinal        float, final time" << std::endl
            << "  ntdump:       int, number of dumps to make between t=[0,tfinal]. If ntdump==1" << std::endl
	    << "                only t=tfinal is dumped. If ntdump>1 t=0 will always be dumped," << std::endl
	    << "                but t=final will not be if the total number of time steps is" << std::endl
	    << "                not divisible by ntdump-1." << std::endl
            << "  filebasename: string, output filenames will take the format filebasename-i.dat," << std::endl
	    << "                where i is the time index at each dump (0,1,2...ndump-1)" << std::endl
            << "example: "<< exec_name << " -square -lf 100 1 10 sqare_lf" << std::endl;
} // END Inline help

/**************************************************************************************/
int main(int argc, char **argv) {
  const int num_expected_args(6);
  if (argc!=num_expected_args+1) {
    help(argv[0],num_expected_args);
    return 1;
  }

  // choose wavetype
  const std::string wavetype(argv[1]);
  Function* u0;
  if (wavetype=="-sine") {
    const double wavelength = 0.5;
    const double phase = 0;
    const double amplitude = 1.;
    u0 = new SineWave(wavelength,phase,amplitude);
  }
  else if (wavetype=="-square") {
    const double height= 1;
    const double xleft = -0.25;
    const double xright = 0.25;
    u0 = new SquareWave(height,xleft,xright);
  }
  else {
    std::cout << "Error " << argv[0] << "::main: wavetype option " << wavetype
	      << " not recognized." << std::endl;
    help(argv[0],num_expected_args);
    return 1;
  }
    
  // set up integration boundaries
  const double xmin(-0.5), xmax(0.5), tmin(0), tmax(atof(argv[4]));
  // set up grid dx = 1/nx (because periodic boundary condition u(xmin)==u(xmax))
  const int nx(atoi(argv[3]));
  const double dx((xmax-xmin)/nx);
  // determine max timestep maxdt = CF*dx/a
  const double char_speed(1);
  const double courant_factor(0.5);
  const double maxdt(dx*courant_factor/char_speed);
  const int nt(ceil((tmax-tmin)/maxdt)+1); // this looks wrong but it's right, because I'm
  //                                       counting the original time slice as a time step
  const double dt((tmax-tmin)/(nt-1)); // tmax, tmin, & nt define dt in the FDSolution methods

  // choose numerical scheme
  const std::string numerical_scheme(argv[2]);
  FDSolution* soln;
  if (numerical_scheme=="-ftcs") {
    soln = new AdvectionFTCS(u0,xmin,xmax,nx,
			     tmin,tmax,nt,char_speed);
  }
  else if (numerical_scheme=="-lf") {
    soln = new AdvectionLF(u0,xmin,xmax,nx,
			   tmin,tmax,nt,char_speed);
  }
  else if (numerical_scheme=="-rk3") {
    soln = new AdvectionRK3(u0,xmin,xmax,nx,
			    tmin,tmax,nt,char_speed);
  }
  else {
    std::cout << "Error " << argv[0] << "::main: method option " << numerical_scheme
	      << " not recognized." << std::endl;
    help(argv[0],num_expected_args);
    return 1;
  }
  soln->Integrateu();

  // determine dump times (or number of time steps between dumps)
  const int dump_nt(atoi(argv[5]));
  int dump_dn; // step size in time index between dumps
  if (dump_nt>1 && dump_nt<=nt) dump_dn = (nt-1)/(dump_nt-1);
  else if (dump_nt==1) dump_dn = nt; // eliminates nan from division by zero above
  else {
    std::cout << "Error " << argv[0] << "::main: ntdump option (" << dump_nt << ") "
	      << "not possible. Must be less than or equal to the number of integration timesteps ("
	      << nt << ")." << std::endl;
    return 1;
  }

  // mine the numerical solution u(x,t), for dump times t, and dump along with the
  //   analytic solution u0(x-at)
  const std::string base_name(argv[6]);
  int n;
  if (dump_nt==1) n=nt-1; // want to dump only the final time if dump_nt==1
  else n=0;
  std::cout << "dump_nt is " << dump_nt << std::endl;
  for(; n<nt; n+=dump_dn) {
    std::cout << "n is " << n << std::endl;
    std::stringstream file_name;
    file_name << base_name << "-" << n << ".dat";
    const double t(tmin+n*dt);
    std::stringstream header;
    header << "# Output from " << argv[0] << " " << wavetype << " " << numerical_scheme << " "
	   << tmax << " " << nx << " " << dump_nt << " " << base_name << std::endl
	   << "#   time = " << t << std::endl
	   << "# [1] x" << std::endl
	   << "# [2] u (numerical)" << std::endl
	   << "# [3] u (analytic)" << std::endl;
    OneDimDataWriter fout(file_name.str(),header.str());
    for(int i=0; i<nx; ++i) {
      const double x(xmin+i*dx);
      double wrappedx(x-char_speed*t); // enforcing hacky periodic abscissa
      while(wrappedx<xmin) wrappedx+=(xmax-xmin);
      while(wrappedx>=xmax) wrappedx-=(xmax-xmin);
      fout.AppendToFile(x,
			soln->getu(i,n),
			u0->f(wrappedx));
    } // END for i (space loop)
  } // END for n (time loop)

  delete u0;
  delete soln;
  return 0;
} // END main
