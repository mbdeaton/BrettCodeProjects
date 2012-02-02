#include "MonteCarloVegasNAI.cpp"

// #include "Utils/StringParsing/OptionParser.hpp"

#include <string>

MonteCarloVegasNAI::MonteCarloVegasNAI(const std::string& opts)
  : NeutrinoAnnihilationIntegral(const std::string& opts) {
}
  
double& MonteCarloVegasNAI::GetQ() {
  return 1; // TODO (Brett): implement correct Q calculation
}

double& MonteCarloVegasNAI::GetQErrorEstimate() {
  return 0.1; // TODO (Brett): implement correct \sigma_Q calculation
}

