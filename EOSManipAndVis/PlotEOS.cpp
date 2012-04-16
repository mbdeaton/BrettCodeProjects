// Brett Deaton
// June 2011
// Prints a table representing the cold beta equilibrium eos.
//   log spacing in rho. Prints: rho, eps, P, dP/drho, h, cs

#include "Hydro/HydroStates/TOVSolution.hpp"
#include "Hydro/EquationOfState/NeosEquationOfState.hpp"
#include "Hydro/EquationOfState/NeosGlobalState.hpp"
#include "Hydro/EquationOfState/NeosPhysicalState.hpp"
#include "Utils/StringParsing/OptionParser.hpp"
#include "Utils/StringParsing/ReadFileIntoString.hpp"
#include <iomanip>

int main()
{
  std::string opts = ReadFileIntoString("input/PlotEOS.input");
  OptionParser p(opts);

  std::string EoSopts = p.Get<std::string>("EoS");
  int N = p.Get<int>("N");
  double rho_min = p.Get<double>("RhoMin");
  double rho_max = p.Get<double>("RhoMax");

  double logmin = log10(rho_min), logmax = log10(rho_max);
  double logrho, rho;
  ofstream plotout("ploteos.dat");
  const NeosEquationOfState mEoS(EoSopts);

  plotout << std::setprecision(15);
  plotout << "# PlotEOS table\n" <<
    "# " << EoSopts << "\n" <<
    "# Units are geometric, with M_sol=1.\n" <<
    "# Conversion to cgs: rhoscale=6.177e17, Energypermassscale=8.989e20, Pscale=5.552e38\n" <<
    "#[1] rho\n" <<
    "#[2] eps, specific energy (E/rho)\n" <<
    "#[3] P\n" <<
    "#[4] dP/drho\n" <<
    "#[5] h, specific enthalpy (1+eps+P/rho)\n" <<
    "#[6] cs, adiabatic relativistic sound speed (nonrelcs/sqrt(h))\n";

  const double minTemp(mEoS.GetGlobalState().MinValidT());
  // below commented out before transition to neos eos framework
  //  const double minYe(mEoS.GetGlobalState().MinValidYe());
  const double minYe(0.15);
  for(int i=0; i<N; i++) {
    logrho = logmin + (double(i)/(N-1.0))*(logmax - logmin);
    rho = pow(10,logrho);
    const NeosPhysicalState mps = mEoS.CreatePhysicalState(rho,minTemp,minYe);
    plotout <<
      rho << "  " <<
      mps.EnergyDensity() << "  " <<
      mps.Pressure() << "  " <<
      mps.dPdrho0() << "  " <<
      mps.Enthalpy() << "  " <<
      mps.SoundSpeed() << "  " << "\n";
  }

  return 1;
}
