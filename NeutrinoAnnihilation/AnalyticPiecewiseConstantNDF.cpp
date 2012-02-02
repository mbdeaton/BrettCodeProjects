#include "Utils/StringParsing/OptionParser.hpp"

#include <string>

AnalyticPiecewiseConstantNDF::AnalyticPiecewiseConstantNDF(const std::string& opts)
  : NeutrinoDistributionFunction(const std::string& opts) {
  OptionParser parser(opts,Help());
  E_break_ = parser.Get<double>("EnergyBreak");
  theta_half_opening_angle_ = parser.Get<double>("HalfOpeningAngle");
}
  
  /**
   * Return f(theta,phi,E). f is normalized by a factor of h^3, to bring values toward unity.
   *
   * @param \theta   polar angle
   * @param \phi     azimuthal angle
   * @param E        neutrino energy
   * @return         f, in erg^-3 s^-3
   */
double& AnalyticPiecewiseConstantNDF::Getf(const double& theta,
					   const double& phi,
					   const double& E) {
  return 1; // TODO (Brett): put in the correct piecewise form of f
}

 private:
  double E_break_; // max neutrino energy, above which f vanishes (MeV)
  double theta_half_opening_angle_; // half-opening angle into southern hemisphere, outside
  //                                   of which f vanishes.
};

#endif  // ANALYTICPIECEWISECONSTANTNDF_HPP_

