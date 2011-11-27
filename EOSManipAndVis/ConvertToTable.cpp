#include "Hydro/HydroComputeItems/EquationOfState.hpp"
#include "Utils/StringParsing/OptionParser.hpp"
#include "Utils/StringParsing/ReadFileIntoString.hpp"

#include <cmath>

// 6.27.11
// Brett Deaton
// Prints a table in ColdTable format from any other EOS class.
//   Useful to make a faster version of my SpectralFitEOS.
//   I'm using geometric units.
int main()
{
  std::string opts = ReadFileIntoString("input/ConvertToTable.input");
  OptionParser p(opts);

  std::string EoSopts = p.Get<std::string>("EoS");
  bool makegnuplotTable = p.Get<bool>("MakeTableForGnuplotInsteadOfColdTableEOSClass");
  int Nrho = p.Get<int>("N");
  double rhomin = p.Get<double>("RhoMin");
  double rhomax = p.Get<double>("RhoMax");

  EquationOfState* mEoS = CreateEquationOfState(EoSopts);

  double HeatCapacity = 1e-3; // this is for T extrapolation
  double GammaTh = 2;
  double kappa = 100;
  std::string RhoSpacing = "Log";
  double mlrho, lrhomin = log10(rhomin), lrhomax = log10(rhomax);
  ofstream tablout("converttotable.dat");

  std::string commentChar = "";
  if(makegnuplotTable) commentChar = "#";
  
  // Print table header                                                                           
  tablout << commentChar << "EoSType = ColdTable\n"
          << commentChar << "Nrho = " << Nrho << "\n"
          << commentChar << "rhomin = " << rhomin << "\trhomax = " << rhomax << "\n"
          << commentChar << "HeatCapacity = " << HeatCapacity << "\n"
          << commentChar << "GammaTh = " << GammaTh << "\n"
          << commentChar << "kappa = " << kappa << "\n"
          << commentChar << "RhoSpacing = " << RhoSpacing << "\n";
  
  // Print table                                                                                  
  for(int i=0; i<Nrho; ++i)
    {
      mlrho = lrhomin + (double(i) / (Nrho-1.0)) * (lrhomax - lrhomin);
      mEoS->set_primitives(pow(10,mlrho)); mEoS->SetRhoDerivs();
      if(makegnuplotTable) tablout << pow(10,mlrho) << "   ";
      tablout << mEoS->Epsilon() << "   "
              << log10(mEoS->Pressure() / kappa) / mlrho << "   "
              << mEoS->SoundSpeed() << "\n";
    }
  
  delete mEoS;
  
  return 1;
}
