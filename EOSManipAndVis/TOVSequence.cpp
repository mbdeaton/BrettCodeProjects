#include "Hydro/HydroStates/TOVSolution.hpp"
#include "Utils/StringParsing/OptionParser.hpp"
#include "Utils/StringParsing/ReadFileIntoString.hpp"

int main()
{
  std::string opts = ReadFileIntoString("input/TOVSequence.input");
  OptionParser p(opts);

  std::string EoSopts = p.Get<std::string>("EoS");
  int N = p.Get<int>("N");
  double rho_min = p.Get<double>("RhoMin");
  double rho_max = p.Get<double>("RhoMax");

  int dim = 3;
  double Scale = 100.0, Pdeplete = 1.0;
  double lnmin = log(rho_min), lnmax = log(rho_max);
  ofstream seqout("tovseq.dat");
  TOVSolution* profiles;

  seqout << "#Output from TOVSequence" << std::endl
	 << "# [1] rho_c (baryon)" << std::endl
	 << "# [2] M (gravitational)" << std::endl
	 << "# [3] r_star (schwarzschild)" << std::endl;
  for(int i=0; i<N; i++) {
    double lnrho = lnmin + (double(i)/(N-1.0))*(lnmax - lnmin);
    double rho_c = exp(lnrho);
    profiles = new TOVSolution(rho_c,dim,EoSopts,Scale,Pdeplete,true);
    double M = profiles->GetMass();
    double R = profiles->GetCircRadius();
    seqout << rho_c << "  " << M << "  " << R << std::endl;
    delete profiles;
  }

  return 0;
}
