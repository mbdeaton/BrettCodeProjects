#include "Hydro/HydroStates/TOVSolution.hpp"
#include "Utils/Tensor/Tensor.hpp"
#include "Utils/DataMesh/DataMesh.hpp"
#include "Utils/StringParsing/OptionParser.hpp"
#include "Utils/StringParsing/ReadFileIntoString.hpp"

#include <cmath>

// 9.7.11
// Brett Deaton
// Prints simple stellar paramters (same as TOVSequence, but with M_baryon and Compactness).
//   Printed parameters: rhoc (baryon), M_baryon, M_adm, R_iso, R_shw, C=M_adm/R_shw
int main()
{
  std::string opts = ReadFileIntoString("input/StellarParameters.input");
  OptionParser p(opts);

  std::string EoSopts = p.Get<std::string>("EoS");
  int numRadPoints = p.Get<int>("NumRadialPoints"); // sets numerical accuracy for M_baryon
  double rho_c = p.Get<double>("CentralDensity"); // set central baryon density

  int dim = 3;
  double Scale = 100.0, Pdeplete = 1.0;
  TOVSolution profile(rho_c,dim,EoSopts,Scale,Pdeplete);
  ofstream paramsout("params.dat");

  IPoint ext(MV::fill,numRadPoints); // defines the extents for the 1D grid
  Mesh mm(ext);
  DataMesh mr(mm); // DataMesh corresponding to a 1D grid, with the above extents
  Tensor<DataMesh> mRho(1,"",mr,0); // Rho = sqrt(detg) * W * Rho0Phys
  Tensor<DataMesh> mP(1,"",mr,0); // Pressure
  DataMesh mPsi4(mr,0); // Conformal Factor
    
  double isorad = profile.GetRadius();
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
  
  if(type=="RhoAndPressure")
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
  else if(type=="ForTOVAddGridRadius")
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
