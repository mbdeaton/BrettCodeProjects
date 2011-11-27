#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

// 6.26.11
// Brett Deaton
// Integrates a table of P(rho) to get eps(rho), where eps is specific internal energy
//   Input table must be in format:
//   Nrho
//   epsAtrho0
//   rho0    P0
//   rho1    P1
//   .      .
//   An output table will be sent to stdout of format:
//   rho0    eps0    P0
//   rho1    eps1    P1
//   .      .      .
int main()
{
  // setup
  int nr=0;
  double epsatrho0;
  string eosfilename = "LS_epsPcs.input";
  //  ifstream eosfile(eosfilename.c_str());
  //  ifstream eosfile(eosfilename.c_str());
  if(!eosfile) cout << "Error: Could not find EOS input file.\n";
  eosfile >> nr;
  eosfile >> epsatrho0;
  vector<double> rho(nr);
  vector<double> P(nr);
  vector<double> eps(nr);
  for(int i=0;i<nr;++i)
    {
      eosfile >> rho[i];
      eosfile >> P[i];
    }

  // Integrate
  eps[0]=epsatrho0;
  for(int i=1;i<nr;++i)
      eps[i] = eps[i-1] +
	( P[i-1]/rho[i-1]/rho[i-1] + P[i]/rho[i]/rho[i] ) *
	(rho[i]-rho[i-1]) / 2;

  // Print
  for(int i=1;i<nr;++i)
    {
      cout << rho[i] << "\t"
	   << eps[i] << "\t"
	   << P[i] << "\n";
    }
  
  return 1;
}
