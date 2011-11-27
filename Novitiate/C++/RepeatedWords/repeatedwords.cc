// Determine if a word has been repeated in input
// Brett Deaton
// 5.25.10

#include <iostream>
#include <string>

using namespace std;

int main()
{
  int n=0;
  string current;
  string previous="";
  while (cin>>current)
    {
      ++n;
      if (previous==current)
	cout << "Word number " << n << "repeated" << current << endl;
      current = previous;
    }
}
