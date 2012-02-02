#ifndef ANALYTICPIECEWISECONSTANTNDF_HPP_
#define ANALYTICPIECEWISECONSTANTNDF_HPP_

/**
 * Piecewise constant neutrino distribution function. For testing purposes.
 * In this implementation, f is position-independent, and assumed to be in
 * the northern hemisphere (which only affects which values of theta yield nonzero
 * values of f). f is azimuthally symmetric.
 *
 * @author M. Brett Deaton \<brett.deaton@gmail.com\>
 * @date 2012-2-2
 */
class AnalyticPiecewiseConstantNDF : public NeutrinoDistributionFunction {
 public:
  AnalyticPiecewiseConstantNDF(const std::string& opts);
  ~AnalyticPiecewiseConstantNDF() {};
  
  /**
   * Return f(theta,phi,E). f is normalized by a factor of h^3, to bring values toward unity.
   *
   * @param \theta   polar angle
   * @param \phi     azimuthal angle
   * @param E        neutrino energy
   * @return         f, in erg^-3 s^-3
   */
  double& Getf(const double& theta, const double& phi, const double& E);

 private:
  double E_break_; // max neutrino energy, above which f vanishes
  double theta_half_opening_angle_; // half-opening angle into southern hemisphere, outside
  //                                   of which f vanishes.
};

#endif  // ANALYTICPIECEWISECONSTANTNDF_HPP_

