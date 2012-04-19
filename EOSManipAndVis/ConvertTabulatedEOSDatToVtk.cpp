/*
  ConvertTabulatedEOSDatToVtk.cpp
  Stolen almost entirely from ConvertDatToVtk.cpp by Patrick Calhoun (8-15-2008)

  Brett Deaton
  4.19.12
*/

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include<cstdlib>
#include "Utils/StringParsing/CommandLineParser.hpp"
#include "Utils/ErrorHandling/BasicMpi.hpp"
#include "Utils/ErrorHandling/MpiEnvironment.hpp"
using namespace std;


//new getDataPerTime function
int getDataPerTime(string file)
{
  ifstream inFile(file.c_str());
  if(inFile.is_open())
    {
      int DPT = 0;
      float dummy;
      string line;
      while(inFile.peek() == '#')
	{
	  getline(inFile,line);
	}
      getline(inFile,line);
      while(line.find('.') != string::npos)
	{
	  string::iterator decimalPlace;
	  decimalPlace = line.begin() + line.find('.');
	  line.erase(decimalPlace);
	  DPT++;
	}
      inFile.close();
      return DPT-1;
    }
  cout << "Error: could not open file: " << file << endl;
  exit(1);
}

/*
//old getDataPerTime

//determines the amount of columns per time (in other words, the total number of columns minus 1 from the time column)
int getDataPerTime(string file)
{
  ifstream inFile(file.c_str());
  if(inFile.is_open())
    {
      int DPT;
      string line;
      getline(inFile,line);
      while(line.find("#") != -1)
	{
	  int beginning = line.find("[") + 1;
	  int length = line.find("]") - beginning;
	  
	  line = line.substr(beginning,length);
	  
	  DPT = atoi(line.c_str())-1; 
	  // -1 since time counts as one of the DPT
	  getline(inFile,line);
	}
      inFile.close();
      return DPT;
    }
  cout << "Error: could not open file" << endl;
  exit(1);
}
*/


//this function returns a vector containing all of the Data of the specified dat file.
vector<float> getData(string file)
{ 
  ifstream inFile(file.c_str());
  if(inFile.is_open())
  {
    vector<float> data(0);
    string line = "";
     while(inFile.peek() == '#')
      {
	getline(inFile,line);
      }
    float component;
    inFile >> component;
    while(!inFile.eof())
      {
	data.push_back(component);
	inFile >> component;
      }
    inFile.close();
    return data;
  }
  cout << "Error: could not open file: " << file << endl;
 exit(1);
}


//MAIN FUNCTION
//######################################################################################
//######################################################################################
int main(int clas, char* args[])
{
  MpiInit(&clas, &args);
  REQUIRE(1==MpiEnvironment::GlobalSize(),
	  "This routine was written as a serial program.  It is unknown if\n"
	  "(and therefore unlikely that) it runs correctly in parallel.\n"
	  "If you want this routine to run in parallel, please (upgrade and)\n"
	  "thoroughly test that it works correctly on multiple processors.\n");
  string Help =
    "Usage: ConvertDatToVtk -d DELTA -o OFFSET -s SCALE DatFile1 DatFile2...     \n"
    "       ConvertDatToVtk takes in wave data from .dat files and displays this \n"
    "       data as a set functions vs time.                                     \n"
    "                                                                            \n"
    "OPTIONS:                                                                    \n"
    "  -d Delta   --  within a single dat file, this is the displacement between \n"
    "                 each separate set of column data                           \n"
    "  -o Offset  --  if more than one dat file is converted at once, the offset \n"
    "                 will displace each additional dat file's data by this      \n"
    "                 amount from the end of the previous dat files display      \n"
    "  -s Scale   --  changes the scale of the height of the wavefunction        \n"
    "  -c Color   --  number used for Color represents the number of the column  \n"
    "                 that will be used to display color along the function      \n";

  CommandLineParser clp(clas,args,Help);
  MyVector<string> InputString = clp.GetAllArguments();
  REQUIRE(clp.NumberOfArguments()>=1,Help);
  const int scale = clp.Get<int>("s");
  const int delta = clp.Get<int>("d");
  const int additionalOffset = clp.Get<int>("o");
  const bool colorBool = clp.OptionIsDefined("c");
  int offset = 0;
  int currentOffset;

  //this for loop goes through all of the dat files given in the command line
  for(int i=0;i<clp.NumberOfArguments();i++)
    {
      string holder = InputString[i];
      holder.resize(holder.find("."));
      const string name = holder+".vtu";    
      const int DPT = getDataPerTime(InputString[i]);
      const vector<float> data(getData(InputString[i]));
      const int points = (data.size() - (data.size()/(DPT+1)));
      //check for color option
      if(colorBool) 
	{
	  const int column = clp.Get<int>("c")-1;
	  if(column < 0 || column > DPT)
	    {
	      cout << "Error: Column choosen for coloring is not contained with the dat file." << endl;
	      exit(1);
	    }
	}
      //checks if this is first dat file (makes no sense to offset first graph)
      if(i != 0)
	{
	  offset = additionalOffset + currentOffset;//sets the specific offset for each dat file
	}

      //writing to vtk file
      ofstream outFile(name.c_str());
      outFile << "<VTKFile type=\"UnstructuredGrid\">" << endl;
      outFile << "  <UnstructuredGrid>" << endl;
      outFile << "    <Piece NumberOfPoints=\"" << points << "\" NumberOfCells=\"" << DPT << "\">" << endl;
      outFile << "      <Points>" << endl;
      outFile << "        <DataArray NumberOfComponents=\"" << "3" << "\" type=\"Float64\" name=\"Coords\" format=\"ascii\">" << endl;
      for(int d=0;d<DPT;d++)
	{
	  for(int count=0;count<data.size();count=count+1+DPT)
	    {
	      outFile << data[count] << " " << offset + delta*d << " " << data[count+1+d]*scale << endl;
	      //writing points to file
	      currentOffset = offset + delta*d;
	    }
	}
      outFile << "        </DataArray>" << endl;
      outFile << "      </Points>" << endl;
      outFile << "      <Cells>" << endl;
      outFile << "        <DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">" << endl;
      for(int d=0;d<DPT;d++)
	{
	  for(int count=d*(points/DPT);count<(points/DPT)*(1+d);count++)
	  {
	    outFile << count << " " ;
	    //writing connectivity to file
	  }
	  outFile << endl;
	}
      outFile << "        </DataArray>" << endl;
      outFile << "        <DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">" << endl;
      for(int d=1;d<=DPT;d++)
	{
	  outFile << (points/DPT)*d << " ";
	  //writing offsets to file
	}
      outFile << endl;
      outFile << "        </DataArray>" << endl;
      outFile << "        <DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">" << endl;
      for(int d=0;d<DPT;d++)
	{
	  outFile << "4 ";
	  //writing cell types to file (4 is a "VTK_POLY_LINE")
	}
      outFile << endl;
      outFile << "        </DataArray>" << endl;
      outFile << "      </Cells>" << endl;
      
      //Checks the coloring option
      if(colorBool)
	{
	  //writes point data for selected column
	  const int color = clp.Get<int>("c")-1;
	 
	  outFile << "      <PointData Scalars=\"colors\">" << endl;
	  outFile << "        <DataArray type=\"Float32\" Name=\"colors\" format=\"ascii\">" << endl;
	  for(int d=0;d<DPT;d++)
	    {
	      for(int count=color;count<data.size();count=count+DPT+1)
		{
		  outFile << data[count] << " ";
		}
	    }
	  outFile << endl;
	  outFile << "        </DataArray>" << endl;
	  outFile << "      </PointData>" << endl;
	}
      else
	{
	  //if no color option is given, the default color is made from the wave function data
	  outFile << "      <PointData Scalars=\"colors\">" << endl;
	  outFile << "        <DataArray type=\"Float32\" Name=\"colors\" format=\"ascii\">" << endl;
	  for(int d=0;d<DPT;d++)
	    {
	      for(int count=(d+1);count<data.size();count=count+DPT+1)
		{
		  outFile << data[count] << " ";
		}
	    }
	  outFile << endl;
	  outFile << "        </DataArray>" << endl;
	  outFile << "      </PointData>" << endl;
	}
      //writing ending code to vtu files
      outFile << "    </Piece>" << endl;
      outFile << "  </UnstructuredGrid>" << endl;
      outFile << "</VTKFile>" << endl;
      outFile << endl;
      outFile.close();
    }
  MpiFinalize();
  return 0;
}
