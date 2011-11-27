// Exploring Inheritance & Polymorphism: a package tracking/organization system
// Brett Deaton 7.13.10

// Notes: I had intended to create a more robust initialization scheme
//   with the default constructor initializing the data members by
//   prompting the user at the terminal. But this requires more
//   knowledge of input/output & string manipulations than I want to
//   bother with right now.
// To do: figure out string objects and char pointers and all that.
//   Things to look at: getline global function; and strtod & strtol
//   conversion functions. I've commented out the experimental pieces.

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include<iomanip>
#include<string>
#include<vector>
using namespace std;


//********************************************************************//
//************************* Declarations *****************************//
//********************************************************************//

// ******* Virtual Base Package Class *******
class Package {
public:
  Package(string myName, string myAddress, string myCity, string myState, int myZip,
	  string yrName, string yrAddress, string yrCity, string yrState, int yrZip,
	  double myWeight, double myRate);
//   Package() {;}
  ~Package() {;}
  void setWeight(double myWeight);
  void setRate(double myRate);
  virtual double calculateCost() =0;
  void printSender();
  void printReceiver();
  virtual void print() =0; // print everything
protected:
  string sName, sAddress, sCity, sState; // Sender
  string rName, rAddress, rCity, rState; // Receiver
  int sZip, rZip;
  double wt; // weight of package in ounces
  double rt; // shipping cost USD per ounce
};

// ******* Two Day Package Class *******
class TwoDayPackage: public Package {
public:
  TwoDayPackage(string myName, string myAddress, string myCity, string myState, int myZip,
		string yrName, string yrAddress, string yrCity, string yrState, int yrZip,
		double myWeight, double myRate, double myFlatRate)
    : Package(myName, myAddress, myCity, myState, myZip,
	      yrName, yrAddress, yrCity, yrState, yrZip,
	      myWeight, myRate) {flatRate=myFlatRate;}
//   TwoDayPackage() {;}
  ~TwoDayPackage() {;}
  double calculateCost() {return wt*rt+flatRate;}
  void print();
protected:
  double flatRate; // flat rate USD for two-day delivery
};

// ******* Overnight Package Class *******
class OvernightPackage : public Package {
public:
  OvernightPackage(string myName, string myAddress, string myCity, string myState, int myZip,
		   string yrName, string yrAddress, string yrCity, string yrState, int yrZip,
		   double myWeight, double myRate, double myFee)
    : Package(myName, myAddress, myCity, myState, myZip,
	      yrName, yrAddress, yrCity, yrState, yrZip,
	      myWeight, myRate) {fee=myFee;}
//   OvernightPackage() {;}
  ~OvernightPackage() {;}
  double calculateCost() {return wt*(rt+fee);}
  void print();
protected:
  double fee; // additional fee USD per ounce for overnight delivery
};


//********************************************************************//
//************************* Definitions ******************************//
//********************************************************************//

// Constructor (by argument)
Package::Package(string myName, string myAddress, string myCity, string myState, int myZip,
		 string yrName, string yrAddress, string yrCity, string yrState, int yrZip,
		 double myWeight, double myRate) {
  this->setWeight(myWeight); this->setRate(myRate);
  sName=myName; sAddress=myAddress; sCity=myCity; sState=myState; sZip=myZip;
  rName=yrName; rAddress=yrAddress; rCity=yrCity; rState=yrState; rZip=yrZip;
}

// // Constructor (by prompt)
// Package::Package() {
//   char tmp[5]; char * endpntr; // I don't actually use the end pointer here
//   cout << "Enter sender's name: "; getline(cin,sName,'\n');
//   cout << "Enter sender's street address: "; getline(cin,sAddress,'\n');
//   cout << "Enter sender's city: "; getline(cin,sCity,'\n');
//   cout << "Enter sender's state: "; getline(cin,sState,'\n');
//   cout << "Enter sender's zip code: "; getline(cin,tmp,'\n');
//   sZip=strtol(tmp,endpntr,10);
//   cout << "Enter receiver's name: "; getline(cin,rName,'\n');
//   cout << "Enter receiver's street address: "; getline(cin,rAddress,'\n');
//   cout << "Enter receiver's city: "; getline(cin,rCity,'\n');
//   cout << "Enter receiver's state: "; getline(cin,rState,'\n');
//   cout << "Enter receiver's zip code: ";  getline(cin,tmp,'\n');
//   rZip=strtol(tmp,endpntr,10);
//   // set weight and rate
//   cout << "Enter package weight in ounces: "; getline(cin,tmp,'\n');
//   this->setWeight(strtod(tmp,endpntr));
//   cout << "Enter shipping rate per ounce in USD: "; getline(cin,tmp,'\n');
//   this->setRate(strtod(tmp,endpntr));
// }

void Package::setWeight(double myWeight) {
  if (myWeight>=0) wt=myWeight;
  else {
    wt=0.0;
    cout << "Error: weight isn't realistic. Initialized to 0.0 oz." << endl;
  }
}

void Package::setRate(double myRate) {
  if (myRate>=0) rt=myRate;
  else {
    rt=0;
    cout << "Error: rate isn't realistic. Initialized to $0.00/oz." << endl;
  }
}

void Package::printSender() {
  cout << " Sender:" << endl;
  cout << sName; cout << endl;
  cout << sAddress; cout << endl;
  cout << sCity; cout << ", ";
  cout << sState; cout << ", ";
  cout << sZip; cout << endl;
}

void Package::printReceiver() {
  cout << " Receiver:" << endl;
  cout << rName; cout << endl;
  cout << rAddress; cout << endl;
  cout << rCity; cout << ", ";
  cout << rState; cout << ", ";
  cout << rZip; cout << endl;
}

// // Constructor (by prompt)
// TwoDayPackage::TwoDayPackage() : Package() {
//   string tmp, end;
//   cout << "Enter additional flat rate in USD: ";
//   getline(cin,tmp,'\n');
//   flatRate=strtod(tmp,&end);
// }

void TwoDayPackage::print() {
  this->printSender(); this->printReceiver();
  cout << " Weight: " << wt << " oz" << endl;
  cout << " Shipping Rate: $" << rt << endl;
  cout << " Additional flat rate: $" << flatRate << endl;
  // set some output specifiers that I stole from the internet (to display money)
  cout.setf(ios::fixed); cout.setf(ios::showpoint); cout << setprecision(2);
  cout << " Shipping cost: \t\t\t$" << this->calculateCost() << endl;
}

// // Constructor (by prompt)
// OvernightPackage::OvernightPackage() : Package() {
//   cout << "Enter additional fee per ounce in USD: ";
//   cin >> fee;
// }

void OvernightPackage::print() {
  this->printSender(); this->printReceiver();
  cout << " Weight: " << wt << " oz" << endl;
  cout << " Shipping Rate: $" << rt << endl;
  cout << " Additional fee per ounce: $" << fee << endl;
  // set some output specifiers that I stole from the internet (to display money)
  cout.setf(ios::fixed); cout.setf(ios::showpoint); cout << setprecision(2);
  cout << " Shipping cost: \t\t\t$" << this->calculateCost() << endl;
}



//********************************************************************//
//***************************** MAIN *********************************//
//********************************************************************//

int main() {
  // create a bunch of packages
  TwoDayPackage myFirstPackage("Brett Deaton","1645 N.E. Merman Dr. Apt C303","Pullman","WA",99163,
			       "Dan Helms","PO Box 639","Buena Vista","CO",81211,
			       21.2, 0.25, 3.5);
  TwoDayPackage mySecondPackage("Brett Deaton","1645 N.E. Merman Dr. Apt C303","Pullman","WA",99163,
				"Michael Deaton","998 Rosedale Dr.","Harrisonburg","VA",22801,
				0.9, 0.25, 3.5);
  OvernightPackage myThirdPackage("Brett Deaton","1645 N.E. Merman Dr. Apt C303","Pullman","WA",99163,
				  "Kim Jong Il","The Palace","Pyongyang","NK",77777,
				  55.3, 0.25, 1.15);
  
  // create a vector of pointers to the Package objects
  vector<Package*> myShipment (3);
  myShipment[0]=&myFirstPackage;
  myShipment[1]=&mySecondPackage;
  myShipment[2]=&myThirdPackage;
  
  double totalCost = 0.0;
  // set some output specifiers that I stole from the internet (to display money)
  cout.setf(ios::fixed); cout.setf(ios::showpoint); cout << setprecision(2);
  for (int i=0; i<myShipment.size(); i++) {
    cout << i+1 << ") --------------------------------------------" << endl;
    myShipment[i]->print();
    totalCost += myShipment[i]->calculateCost();
  }
  cout << "-----------------------------------------------" << endl;
  cout << "You owe:\t\t\t\t$" << totalCost << endl;

  return 0;
}
