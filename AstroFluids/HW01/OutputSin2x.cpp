// OutputSin2x.cpp
// Astrophysical Fluid Dynamics: HW01
//   A program to output sin^2(x) on the range [0,2pi]
//   Brett Deaton 8.31.11

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

/*************************************************************************/
// Idea sort of stolen from SpEC Class 2011 (Harald Pfeiffer) because this is a useful class
//   to copy and paste into future assignments.
class OneDimDataWriter {
public:
  /* Constructor:
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
private:
  std::ofstream mOut;
}; // END class OneDimDataWriter

/*************************************************************************/
double f(const double& x) {return pow(sin(x),2.);}

/*************************************************************************/
void help() {
  std::cout << "ERROR! This executable requires two arguments at the command line." << std::endl
	    << "  use: OutputSin2x <int num_ordinate> <string filename>" << std::endl
	    << "    num_ordinate = x-axis resolution" << std::endl
	    << "    filename     = name of output dat file w/ extension" << std::endl
	    << "  example: OutputSin2x 300 sin2x.dat" << std::endl;
}

/*************************************************************************/
int main(int argc, char **argv) {
  if (argc!=3) {
    help();
    return 1;
  }
  // set up
  const double pi = 4.*atan(1);
  const double xmin = 0.;
  const double xmax = 2.*pi;
  const double xrange = xmax-xmin;
  const double n_ord = atof(argv[1]);
  double x;
  const std::string file_name = argv[2];
  std::stringstream mheader;
  mheader << "# Output from " << argv[0] << " executable\n" <<
    "#   x-range      = [" << xmin << "," << xmax << "]\n" <<
    "#   x-resolution = " << n_ord << "\n" <<
    "#[1] x\n#[2] sin^2(x)\n";
  OneDimDataWriter plotout(file_name,mheader.str());
  
  // compute function and print
  for(int i=0; i<n_ord; ++i) {
    x = xmin + xrange*double(i)/(n_ord-1);
    plotout.AppendToFile(x,f(x));
  }

  return 0;
}
