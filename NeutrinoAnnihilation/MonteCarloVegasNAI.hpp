#ifndef MONTECARLOVEGASNAI_HPP_
#define MONTECARLOVEGASNAI_HPP_

template <typename> class MyVector;
template <typename> class CODValue; // compute on demand object

/**
 * Vegas Monte Carlo method implemented to sum the neutrino annihilation integral
 * defined in class NeutrinoAnnihilationIntegral.
 *
 * @author M. Brett Deaton \<brett.deaton@gmail.com\>
 * @date 2012-2-2
 */
class MonteCarloVegasNAI : public NeutrinoAnnihilationIntegral {
 public:
  MonteCarloVegasNAI(const std::string& opts);
  ~MonteCarloVegasNAI() {};
  
  /**
   * Return the heating due to neutrino/antineutrino annihilation at a single
   * point in space. Q is the local rate of heating per volume,
   * in units of erg/cm^3/s. Calculated in the locally nonrotating tetrad frame, as
   * defined in Harikae et al. 2010, eqn 3.
   *
   * @return Q, in erg/cm^3/s
   */
  double& GetQ();

  /**
   * Return the estimated error/uncertainty in Q. See above GetQ().
   *
   * @return \sigma_Q, in erg/cm^3/s
   */
  double& GetQErrorEstimate();

  /**
   * Accessors
   */

 private:
};

#endif  // MONTECARLOVEGASNAI_HPP_

