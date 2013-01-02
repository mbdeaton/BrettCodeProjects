#include "Hydro/HydroStates/TOVSolution.hpp"
#include "Utils/Tensor/Tensor.hpp"
#include "Utils/DataMesh/DataMesh.hpp"
#include "Utils/StringParsing/OptionParser.hpp"
#include "Utils/StringParsing/ReadFileIntoString.hpp"

#include <cmath>
#include <iostream>
#include <iomanip>

// 6.22.11
// Brett Deaton
// Prints a baryon density & pressure profile of a TOV star at evenly spaced
//   points in isotropic radius.
int main()
{
  std::string opts = ReadFileIntoString("input/StellarProfile.input");
  OptionParser p(opts);
  
  std::string EoSopts = p.Get<std::string>("EoS");
  int numRadPoints = p.Get<int>("NumRadialPoints");
  double rho_c = p.Get<double>("CentralDensity");
  std::string table_type = p.Get<std::string>("TableType");
  bool output_high_res = p.Get<bool>("OutputHighResolution");

  int dim = 3;
  int precision = 6;
  if (output_high_res) precision = 15;
  const double Pdeplete = 1.0;
  TOVSolution profile(rho_c,dim,EoSopts,Pdeplete,true);
  ofstream profout("profile.dat");
  profout << setprecision(precision);
  
  IPoint ext(MV::fill,numRadPoints); // defines the extents for the 1D grid
  Mesh mm(ext);
  DataMesh mr(mm); // DataMesh corresponding to a 1D grid, with the above extents
  Tensor<DataMesh> mD(1,"",mr,0); // Density = rho * sqrt(detg) * W
  Tensor<DataMesh> mP(1,"",mr,0); // Pressure
  DataMesh mPsi4(mr,0); // Conformal Factor
  
  double rad = profile.GetRadius();
  for (int i=0; i<numRadPoints; ++i)
    mr[i]= (double(i)/(numRadPoints-1.0))*rad; // populate the mesh with radial points
  
  profile.Density(mr,mD);
  profile.Pressure(mr,mP);
  profile.ConformalFactor(mr,mPsi4);
  
  /* baryon density = D / Psi4^(1.5)
     This is derived from the following facts and definitions:
     the fluid and the metric are stationary and spherically symmetric
     u_a * u^a == -1
     g_{tt} = -Lapse^2 // the 4 metric
     gamma_{ij} == Psi4 * delta_{ij} // the 3 metric
  */
  
  if(table_type=="RhoAndPressure")
    {
      profout << "# " << EoSopts << "\n";
      profout << "# rho_c = " << rho_c << "\n";
      profout << "# r (iso) = " << rad << "\n";
      profout << "#[1] r (isotropic coords)\n#[2] rho (baryon density)\n#[3] P\n";
      for (int i=0; i<numRadPoints; ++i)
 	profout << mr[i] << "   "
 		<< mD()[i] / pow(mPsi4[i],1.5) << "   "
		<< mP()[i] << "\n";
    }
  else if(table_type=="ForTOVAddGridRadius")
    {
      profout << "# " << EoSopts << "\n";
      profout << "# rho_c = " << rho_c << "\n";
      profout << "# r (iso) = " << rad << "\n";
      profout << "#[1] r (isotropic coords)\n#[2] rho (baryon density)\n";
      profout << numRadPoints << "\n";
      for (int i=0; i<numRadPoints; ++i)
 	profout << mr[i] << "   "
		<< mD()[i] / pow(mPsi4[i],1.5) << "\n";
    }
  else
    {
      profout << "Error: TableType option is gibberish" << std::endl;
      return 0;
    }
  
  return 1;
}
