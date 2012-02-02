#ifndef NEUTRINOANNIHILATIONINTEGRAL_HPP_
#define NEUTRINOANNIHILATIONINTEGRAL_HPP_

template <typename> class MyVector;
class NeutrinoDistributionFunction;
template <typename> class CODValue; // compute on demand object

/**
 * Computes energy deposition due to neutrino-antineutrino annihilation (currently,
 * only electron neutrinos are implemented) at requested points in a 3-dimensional
 * volume.
 *
 * @author M. Brett Deaton \<brett.deaton@gmail.com\>
 * @date 2012-2-1
 */
class NeutrinoAnnihilationIntegral {
 public:
  NeutrinoAnnihilationIntegral(const std::string& opts);
  virtual ~NeutrinoAnnihilationIntegral() {};
  
  /**
   * Return the heating due to neutrino/antineutrino annihilation at a single
   * point in space. Q is the local rate of heating per volume,
   * in units of erg/cm^3/s. Calculated in the locally nonrotating tetrad frame, as
   * defined in Harikae et al. 2010, eqn 3.
   *
   * @return Q, in erg/cm^3/s
   */
  virtual double& GetQ() =0;

  /**
   * Return the estimated error/uncertainty in Q. See above GetQ().
   *
   * @return \sigma_Q, in erg/cm^3/s
   */
  virtual double& GetQErrorEstimate() =0;

 private:
  NeutrinoDistributionFunction* f_;
  MyVector<double> position_; // x,y,z location at which integral is computed, grid coords
  CODValue<double> cod_Q_; // heating rate per volume (erg/cm^3/s)
  CODValue<double> cod_sigmaQ_; // estimated uncertainty in Q (erg/cm^3/s)
};

#endif  // NEUTRINOANNIHILATIONINTEGRAL_HPP_

