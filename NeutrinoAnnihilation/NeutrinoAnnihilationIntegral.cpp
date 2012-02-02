#include "NeutrinoAnnihilationIntegral.hpp"
#include "Hydro/EquationOfState/CODValue.hpp"
#include "Utils/StringParsing/OptionParser.hpp"

NeutrinoAnnihilationIntegral::NeutrinoAnnihilationIntegral(const std::string& opts) {
  OptionParser parser(opts,Help());
  target_accuracy_ = parser.Get<double>("Accuracy");
  position_ = parser.Get<MyVector<double> >("Position");
  f_ = parser.Get<std::string>("NeutrinoDistributionFunction");
}

