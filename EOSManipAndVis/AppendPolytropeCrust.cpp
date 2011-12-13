// 7.19.11
// Brett Deaton

#include "Utils/LowLevelUtils/ConvertNumberToString.hpp"
#include "Utils/MyContainers/MyVector.hpp"
#include "Utils/StringParsing/OptionParser.hpp"
#include "Utils/StringParsing/ReadFileIntoString.hpp"
#include "Utils/ErrorHandling/Require.hpp"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

// Linear interpolation Functions in the form of y=mx+b, y=ordinate, x=abscissa
double ordinate(const double xi, const double xii,
		const double yi, const double yii,
		const double abscissa) {
  double m = (yii-yi)/(xii-xi);
  double b = yi-m*xi;
  return m*abscissa+b;
}
double abscissa(const double xi, const double xii,
		const double yi, const double yii,
		const double ordinate) {
  double m = (yii-yi)/(xii-xi);
  double b = yi-m*xi;
  return (ordinate-b)/m;
}

// Prints a table in ColdTable format from a ColdTable EOS class
//   after affixing a Gamma=2 polytrope tail for the crust. Useful to explore
//   the idea of chopping off the density tail present in TOV profiles of
//   realistic EOSs.
int main()
{
  std::string opts = ReadFileIntoString("input/AppendPolytropeCrust.input");
  OptionParser p(opts);

  const std::string InputFileName = p.Get<std::string>("TabulatedEoSFile");
  const bool makegnuplotTable = p.Get<bool>("MakeTableForGnuplotInsteadOfColdTableEOSClass");

  // *** Read Input Table Header ***************************************************************
  // *** Read Input Table **********************************************************************
  std::ifstream tablin(InputFileName.c_str());
  REQUIRE(tablin,"Could not find EOS input table.");
  char buf[100], equalsgn;
  std::string EoSType, RhoSpacing;
  double RhoMin, RhoMax, HeatCapacity, GammaTh, tabKappa;
  int Nrho;

  tablin.get(buf,100,'='); tablin.get(equalsgn); tablin >> EoSType;
  REQUIRE(EoSType=="ColdTable","EoSType must be ColdTable.")
  tablin.get(buf,100,'='); tablin.get(equalsgn); tablin >> Nrho;
  tablin.get(buf,100,'='); tablin.get(equalsgn); tablin >> RhoMin;
  tablin.get(buf,100,'='); tablin.get(equalsgn); tablin >> RhoMax;
  tablin.get(buf,100,'='); tablin.get(equalsgn); tablin >> HeatCapacity;
  tablin.get(buf,100,'='); tablin.get(equalsgn); tablin >> GammaTh;
  tablin.get(buf,100,'='); tablin.get(equalsgn); tablin >> tabKappa; // remember:
  //                            tabKappa is defined differently than canonical Kappa below
  tablin.get(buf,100,'='); tablin.get(equalsgn); tablin >> RhoSpacing;
  REQUIRE(RhoSpacing=="Log","Spacing must be logarithmic in rho.");

  const double lRhoMin = log10(RhoMin),
    lRhoMax = log10(RhoMax),
    dlrho = (lRhoMax-lRhoMin)/double(Nrho-1);
  MyVector<double> tabeps(MV::Size(Nrho)),
    tabGamma(MV::Size(Nrho)), // remember tabGamma is defined differently than canonical Gamma below
    tabCs(MV::Size(Nrho));

  for(int k=0; k<Nrho; k++)
    tablin >> tabeps[k] >> tabGamma[k] >> tabCs[k];

  // *** Find rhoBreak, to Append Polytrope *******************************************************
  const double polGamma = 2; // adiabatic index of polytrope being affixed to table
  double lPi, lPii, lPiii; // log10 pressure at the ith, i+1th, and i+2th positions
  double lrhoi, lrhoii, lrhoiii; // log10 density "
  double Gammai, Gammaii; // dlogP/dlogrho "
  //                         I'm using a midpoint deriv, so Gammai corresponds to (lrhoi+lrhoii)/2

  // find the point at which Gamma passes through polGamma
  int i = Nrho-2; // global index for where in the table I'm stitching on the polytrope
  lrhoii = lRhoMax;
  lrhoi = lrhoii - dlrho;
  lPii = log10(tabKappa) + tabGamma[i+1]*lrhoii;
  lPi = log10(tabKappa) + tabGamma[i]*lrhoi;
  Gammai = (lPii-lPi) / dlrho;
  do {
    --i;
    lrhoii = lrhoi;
    lrhoi = lrhoii - dlrho;
    lPii = lPi;
    lPi = log10(tabKappa) + tabGamma[i]*lrhoi;
    Gammai = (lPii-lPi) / dlrho;
  } while (Gammai > polGamma);
  lrhoiii = lrhoii + dlrho;
  lPiii = log10(tabKappa) + tabGamma[i+2]*lrhoiii;
  Gammaii = (lPiii-lPii) / dlrho;
  
  // *** Construct Polytrope *******************************************************************
  // At this point, the densities corresponding to Gammai and Gammaii bracket rhoBreak at
  //   which I'll affix the polytrope.
  double lrhoBreak = abscissa(lrhoi+dlrho/2, lrhoii+dlrho/2,
			      Gammai, Gammaii,
			      polGamma);
  double lPBreak; // interpolated from table; thus I need to know to which side of rhoii rhoBreak falls
  double epsBreak; // interpolated from table
  double polKappa; // chosen for continuity in P
  double epsShift; // chosen for continuity in eps (and thus h & Cs)
  if(lrhoBreak<lrhoii) {
    lPBreak = ordinate(lrhoi, lrhoii,
		       lPi, lPii,
		       lrhoBreak);
    epsBreak = ordinate(lrhoi, lrhoii,
			tabeps[i], tabeps[i+1],
			lrhoBreak);
  }
  else {
    lPBreak = ordinate(lrhoii, lrhoiii,
		       lPii, lPiii,
		       lrhoBreak);
    epsBreak = ordinate(lrhoii, lrhoiii,
			tabeps[i+1], tabeps[i+2],
			lrhoBreak);
  }
  polKappa = pow(10, lPBreak-polGamma*lrhoBreak);
  epsShift = epsBreak - polKappa*pow(pow(10,lrhoBreak),polGamma-1.) / (polGamma-1.);
  std::cout << "polGamma = " << polGamma << "\n";
  std::cout << "polKappa = " << polKappa << "\n";
  std::cout << "epsShift = " << epsShift << "\n";
  std::cout << "lPBreak = " << lPBreak << "\n";
  std::cout << "lrhoBreak = " << lrhoBreak << "\n";
  std::cout << "epsBreak = " << epsBreak << "\n";
  
  // *** Print Appended Table ******************************************************************
  std::ofstream tablout("appendpolytropecrust.dat");
  std::string commentChar = "";
  if(makegnuplotTable) commentChar = "#";
  
  // Print table header
  tablout << std::setprecision(15);
  tablout << commentChar << "EoSType = ColdTable\n"
	  << commentChar << "Nrho = " << Nrho << "\n"
	  << commentChar << "rhomin = " << RhoMin << "\trhomax = " << RhoMax << "\n"
	  << commentChar << "HeatCapacity = " << HeatCapacity << "\n"
	  << commentChar << "GammaTh = " << GammaTh << "\n"
	  << commentChar << "kappa = " << tabKappa << "\n"
	  << commentChar << "RhoSpacing = " << RhoSpacing << "\n";
  if(makegnuplotTable) tablout << "#[1] rho\n#[2] eps\n#[3] Gamma\n#[4] Cs\n#[5] h\n";

  // Print table
  double lrho, eps, Gamma, Cs, h;
  for(int k=0; k<Nrho; ++k)
    {
      lrho = lRhoMin + (double(k) / (Nrho-1.0)) * (lRhoMax - lRhoMin);
      if(makegnuplotTable) tablout << pow(10,lrho) << "   ";
      if(lrho<lrhoBreak) {
	eps = epsShift + polKappa*pow(pow(10,lrho),polGamma-1.)/(polGamma-1.);
	Gamma = polGamma + (log10(polKappa/tabKappa)) / lrho;
	Cs = sqrt( polKappa*polGamma*pow(pow(10,lrho),polGamma-1.) / (1 + polGamma*eps) );
	h = 1 + eps + polKappa*pow(pow(10,lrho),polGamma-1.);
      }
      else {
	eps = tabeps[k];
	Gamma = tabGamma[k];
	Cs = tabCs[k];
	h = 1 + eps + tabKappa*pow(pow(10,lrho),Gamma-1.);
      }
      tablout << eps << "   "
	      << Gamma << "   "
	      << Cs;
      if(makegnuplotTable) tablout << "   " << h;
      tablout << "\n";
    }
  
  return 1;
}
