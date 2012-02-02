#ifndef NEUTRINODISTRIBUTIONFUNCTION_HPP_
#define NEUTRINODISTRIBUTIONFUNCTION_HPP_

#include <string>

template <typename> class MyVector;

/**
 * Abstract base class from which are derived specific implementations of f.
 * f represents a local neutrino or antineutrino distribution function,
 * $\frac{dN}{d^3x d^3p}$. f is normalized by a factor of h^3, to bring values toward unity.
 *
 * @author M. Brett Deaton \<brett.deaton@gmail.com\>
 * @date 2012-2-2
 */
class NeutrinoDistributionFunction {
 public:
  NeutrinoDistributionFunction(const std::string& opts);
  virtual ~NeutrinoDistributionFunction() {};
  
  /**
   * Return f(theta,phi,E). f is normalized by a factor of h^3, to bring values toward unity.
   *
   * @param \theta   polar angle
   * @param \phi     azimuthal angle
   * @param E        neutrino energy
   * @return         f, in erg^-3 s^-3
   */
  virtual double& Getf(const double& theta, const double& phi, const double& E) =0;

  /**
   * Accessors
   */
  double position() {return position_;}
  std::string flavor() {return flavor_;}

 private:
  MyVector<double> position_; // x,y,z location of f, grid coords
  std::string flavor_; // currently implemented: "electron", "antielectron"
  //                      TODO (Brett): build a NeutrinoParams class to house this and other
  //                      flavor-dependent constants
};

#endif  // NEUTRINODISTRIBUTIONFUNCTION_HPP_

