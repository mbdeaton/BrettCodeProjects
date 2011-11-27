// Brett Deaton - 10.25.11
// Sort of modeled on SpEC OneDimDataWriter

#ifndef OneDimDataWriter_H
#define OneDimDataWriter_H

#include<fstream>
#include<iostream>
#include<vector>

// Append Data to a DataFile, useful for one time slice
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
  // AppendToFile: x   abscissa
  //               y   ordinate
  void AppendToFile(const double& x, const double& y) {
    mOut << x << "   " << y << std::endl;
  };
  void AppendToFile(const double& x, const double& y, const double& z) {
    mOut << x << "   " << y << "   " << z << std::endl;
  };
  void AppendToFile(const double& x, const double& y, const double& z, const double& w) {
    mOut << x << "   " << y << "   " << z << "   " << w << std::endl;
  };
  void AppendToFile(const double& x,
		    const double& y1, const double& z1, const double& w1,
		    const double& y2, const double& z2, const double& w2) {
    mOut << x << "   "
	 << y1 << "   " << z1 << "   " << w1 << "   "
	 << y2 << "   " << z2 << "   " << w2 << std::endl;
  };
private:
  std::ofstream mOut;
}; // END Class OneDimDataWriter

// ***********************************************************************************************
// Append Data to a Ygraph file, useful for multiple timesteps
class YgraphDataWriter {
public:
  /*Constructor:
      file_name        name of output file including extension
      header           any comment to be included at the top of the file,
      w/ comment char incl, but not the final endl
  */
  YgraphDataWriter(const std::string file_name,
                   const std::string header):
    mOut(file_name.c_str()) { mOut << header << std::endl;};
  ~YgraphDataWriter() { mOut.close();};
  // Append a new time step to the file: x   abscissa
  //                                     y   ordinate
  void AppendToFile(const double& time, const std::vector<double>& x, const std::vector<double>& y) {
    const int nx((int)x.size());
    if (nx!=(int)y.size())
      std::cout << "Error YgraphDataWriter::AppendToFile: x & y vectors not same length" << std::endl;
    mOut << std::endl << "\"Time = " << time << std::endl;
    for (int i=0; i<nx; ++i) {
      mOut << x[i] << "   " << y[i] << std::endl;
    }
  };
private:
  std::ofstream mOut;
}; // END Class YgraphDataWriter

#endif
