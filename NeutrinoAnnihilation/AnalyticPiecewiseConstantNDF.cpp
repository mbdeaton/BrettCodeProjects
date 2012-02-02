#include "AnalyticPiecewiseConstantNDF.hpp"

#include "Utils/StringParsing/OptionParser.hpp"

#include <string>

AnalyticPiecewiseConstantNDF::AnalyticPiecewiseConstantNDF(const std::string& opts)
  : NeutrinoDistributionFunction(const std::string& opts) {
  OptionParser parser(opts,Help());
  E_break_ = parser.Get<double>("EnergyBreak");
  theta_half_opening_angle_ = parser.Get<double>("HalfOpeningAngle");
}
  
double& AnalyticPiecewiseConstantNDF::Getf(const double& theta,
					   const double& phi,
					   const double& E) {
  return 1; // TODO (Brett): put in the correct piecewise form of f
}
