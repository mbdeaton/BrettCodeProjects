// Displaying the relationship between pointers and arrays
//   Also showing the functionality of dereference (*) & reference (&) operators
// Brett stole from cplusplus.com tutorial

#include <iostream>
using namespace std;

int main ()
{
  int numbers[5];
  int * p;
  p = numbers;  *p = 10;
  p++;  *p = 20;
  p = &numbers[2];  *p = 30;
  p = numbers + 3;  *p = 40;
  p = numbers;  *(p+4) = 50;
  for (int n=0; n<5; n++)
    cout << numbers[n] << ", ";
  cout << "\b" << "\b" << endl;
  return 0;
}
