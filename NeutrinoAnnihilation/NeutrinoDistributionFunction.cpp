#include "NeutrinoDistributionFunction.hpp"
#include "Utils/StringParsing/OptionParser.hpp"

#include <string>

NeutrinoDistributionFunction::NeutrinoDistributionFunction(const std::string& opts)
  : /*TODO (brett): initialize stuff*/ {
  OptionParser parser(opts,Help());
  position_ = parser.Get<MyVector<double> >("Position");
  flavor_ = parser.Get<std::string>("Flavor");
}

