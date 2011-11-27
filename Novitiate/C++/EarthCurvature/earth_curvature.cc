// Calculate the vertical drop of sealevel from the horizon plane
//   given an argument of miles from an observer.
// Brett Deaton
// 5.18.10
//
// The program assumes a spheroidal earth, and is limited to a range of
//   .25 earthcircumferences, because beyond this distance, the meaning
//   of the problem is less clear.
// The equation is the same as that used to describe the potential
//   energy of a simple pendulum (less mg): h = r(1-cos(theta)), where
//   theta = d/r.

#include <iostream>
#include <cmath>
using namespace std;

// Constants
const float  PI = 3.142;
const float RADIUS = 3959; // mean radius of the earth in mi

int main()
{
  float dist = 1;
  float drop = 0;
  
  cout << "\n*****Earth's vertical drop from horizon plane*****";
  cout << "\n****************(enter 0 to quit)*****************";
  while (dist)
    {
      cout << "\nEnter a distance in mi: ";
      cin >> dist;
      if (not dist)
	cout << "\nGoodbye" << endl;
      else if (dist < 0.5*PI*RADIUS)
	{
	  drop = RADIUS*(1-cos(dist/RADIUS))*5280; // in ft
	  cout << dist << " mi away, the earth's surface is\n"
	       << drop << " ft below the horizon plane." << endl;
	}
      else
	cout << "\nDon't be silly." << endl;
    }
  return 0;
}
