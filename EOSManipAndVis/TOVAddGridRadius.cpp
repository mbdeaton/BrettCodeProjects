//============================================================================
// $Id: TOVAddGridRadius.cpp 16941 2011-07-15 Deaton $
//============================================================================
#include "Dust/Domain/CoordMap/CoordMap.hpp"
#include "Dust/Domain/CoordMap/CreateCoordMap.hpp"
#include "Utils/StringParsing/StringUtils.hpp"
#include "Utils/StringParsing/OptionParser.hpp"
#include "Utils/StringParsing/ReadFileIntoString.hpp"
#include "Utils/DataMesh/DataMesh.hpp"

#include <cmath>
#include <iostream>
#include <string>

using namespace CoordMaps;

int main(int argc, char* argv[]) {
  if(argc!=4) {
    std::cout << "Usage:  TOVAddGridRadius  InputFile CoordMapFile OutputFile"
	      << std::endl;
    return 0;
  }

  const std::string Help=
    "CoordMapFile \n"
    "    Map= <string>;\n";

  std::ifstream infile(argv[1]);
  std::ofstream outfile(argv[3]);
  std::string CoordString = ReadFileIntoString(argv[2]);
  OptionParser p(CoordString, Help);

  CoordMap* map=CreateCoordMap(p.Get<std::string>("Map"));

  int N;
  infile >> N;

  for(int i=0; i<N; i++) {
    double ri, rho;
    infile >> ri >> rho; 
    ri = ri;
    
    IPoint Extents(MV::fill,1,1,1);
    const Mesh mesh(Extents);
    MyVector<DataMesh> Xi(MV::Size(3),mesh);
    Xi[0][0] = ri; Xi[1][0] = Xi[2][0] = 0.0;
    MyVector<DataMesh> Xg = map->OrigCoords(Xi);

    outfile << ri << "  "
	    << Xg[0][0] << "  "
	    << rho << "  "
	    << std::endl;
  }
  return 1;
}
