// Brett Deaton - 10.25.11
// Sort of modeled on SpEC OneDimDataWriter

#ifndef OneDimDataWriter_H
#define OneDimDataWriter_H

#include <fstream>

// Append Data to a DataFile                                                                             
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

#endif
