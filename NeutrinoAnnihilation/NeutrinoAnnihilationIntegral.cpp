#include "NeutrinoAnnihilationIntegral.hpp"
#include "Hydro/EquationOfState/CODValue.hpp"
#include "Utils/StringParsing/OptionParser.hpp"

NeutrinoAnnihilationIntegral::NeutrinoAnnihilationIntegral(const std::string& opts)
  : /*TODO (brett): initialize stuff*/ {
  OptionParser parser(opts,Help());
  position_ = parser.Get<MyVector<double> >("Position");
  f_ = parser.Get<std::string>("NeutrinoDistributionFunction");
}

