#include "Hydro/EquationOfState/NeosEquationOfState.hpp"
#include "Hydro/EquationOfState/NeosGlobalState.hpp"
#include "Hydro/EquationOfState/NeosPhysicalState.hpp"
#include "Utils/StringParsing/OptionParser.hpp"
#include "Utils/StringParsing/ReadFileIntoString.hpp"

#include <cmath>
#include <fstream>

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

  const NeosEquationOfState mEoS(EoSopts);

  double HeatCapacity = 1e-3; // this is for T extrapolation
  double GammaTh = 2;
  double kappa = 100;
  std::string RhoSpacing = "Log";
  double mlrho, lrhomin = log10(rhomin), lrhomax = log10(rhomax);
  std::ofstream tablout("converttotable.dat");

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
  
  const double minTemp(mEoS.GetGlobalState().MinValidT());
  // below commented out before transition to neos eos framework
  //  const double minYe(mEoS.GetGlobalState().MinValidYe());
  const double minYe(0.15);

  // Print table                                                                                  
  for(int i=0; i<Nrho; ++i)
    {
      mlrho = lrhomin + (double(i) / (Nrho-1.0)) * (lrhomax - lrhomin);
      const NeosPhysicalState mps = mEoS.CreatePhysicalState(pow(10,mlrho),minTemp,minYe);
      if(makegnuplotTable) tablout << pow(10,mlrho) << "   ";
      tablout << mps.EnergyDensity() << "   "
              << log10(mps.Pressure() / kappa) / mlrho << "   "
              << mps.SoundSpeed() << "\n";
    }
  
  return 1;
}
