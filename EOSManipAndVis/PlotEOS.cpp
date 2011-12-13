// Brett Deaton
// June 2011
// Prints a table representing the cold beta equilibrium eos.
//   log spacing in rho. Prints: rho, eps, P, dP/drho, h, cs

#include "Hydro/HydroStates/TOVSolution.hpp"
#include "Hydro/HydroComputeItems/EquationOfState.hpp"
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
  EquationOfState* mEoS = CreateEquationOfState(EoSopts);

  plotout << std::setprecision(13);
  plotout << "# PlotEOS table\n" <<
    "# " << EoSopts << "\n" <<
    "#[1] rho\n#[2] eps\n#[3] P\n#[4] dP/drho\n#[5] h\n#[6] cs\n";

  for(int i=0; i<N; i++) {
    logrho = logmin + (double(i)/(N-1.0))*(logmax - logmin);
    rho = pow(10,logrho);
    mEoS->set_primitives(rho);
    mEoS->SetRhoDerivs();
    plotout <<
      rho << "  " <<
      mEoS->Epsilon() << "  " <<
      mEoS->Pressure() << "  " <<
      mEoS->dPdrho() << "  " <<
      mEoS->Enthalpy() << "  " <<
      mEoS->SoundSpeed() << "  " << "\n";
  }

  return 1;
}
