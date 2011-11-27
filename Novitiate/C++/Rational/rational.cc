// Rational Class
// Brett Deaton
// 6.29.10

#include <iostream>
#include <cmath>
using namespace std;

class Rational {
public:
  Rational(int, int);
  Rational(int);
  Rational();
  ~Rational() {};
  void print() const;
  void nprint() const;
  double nconvert() const;
  // basic operators
  Rational operator=(const Rational);
  Rational operator=(int);
  Rational operator+(Rational);
  Rational operator+(int);
  Rational operator-(Rational);
  Rational operator-(int);
  Rational operator*(Rational);
  Rational operator*(int);
  Rational operator/(Rational);
  Rational operator/(int);
  // boolean operators
  bool operator==(Rational) const;
  bool operator==(int) const;
protected:
  int num, den; // numerator, denominator
  void set(int, int); // for a clean initiation
  void clean(); // for reducing and moving negatives to numerator
};

Rational::Rational(int numerator, int denominator) {
  this->set(numerator, denominator);
}

Rational::Rational(int a) {
  this->set(a,1);
}

Rational::Rational() {
  this->set(0,1);
}

void Rational::print() const { cout << num << "/" << den; }

void Rational::nprint() const { cout << this->nconvert(); }

double Rational::nconvert() const { return double(num)/double(den); }

Rational Rational::operator=(const Rational other) {
  this->set(other.num,other.den);
  return *this;
}

Rational Rational::operator=(int a) {
  this->set(a,1);
  return *this;
}

Rational Rational::operator+(Rational other) {
  Rational temp;
  temp.set(other.den*num+other.num*den,den*other.den);
  return temp;
}

Rational Rational::operator+(int a) {
  Rational temp;
  temp.set(num+a*den, den);
  return temp;
}

Rational Rational::operator-(Rational other) {
  Rational temp;
  temp.set(other.den*num-other.num*den,den*other.den);
  return temp;
}

Rational Rational::operator-(int a) {
  Rational temp;
  temp.set(num-a*den,den);
  return temp;
}

Rational Rational::operator*(Rational other) {
  Rational temp;
  temp.set(num*other.num,den*other.den);
  return temp;
}

Rational Rational::operator*(int a) {
  Rational temp;
  temp.set(num*a,den);
  return temp;
}

Rational Rational::operator/(Rational other) {
  Rational temp;
  temp.set(num*other.den,den*other.num);
  return temp;
}

Rational Rational::operator/(int a) {
  Rational temp;
  temp.set(num,den*a);
  return temp;
}

bool Rational::operator==(Rational other) const {
  return (this->nconvert()==other.nconvert());
}

bool Rational::operator==(int a) const {
  return (this->nconvert()==a);
}

void Rational::set(int numerator, int denominator) {
  // protect against division by zero
  if (denominator!=0) {
    num = numerator;
    den = denominator;
    this->clean();
  }
  else { cout << "Err: Division by zero. No operation performed." << endl; }  
}

void Rational::clean() {
  // clean up negative signs
  if (den<0) {
    den = -1*den;
    num = -1*num;
  }
  // reduce the fraction
  if (num!=0) { // don't enter this loop if the rational is zero
    int gcf=1;
    int smallest;
    if (abs(num)<abs(den)) { smallest=abs(num); }
    else { smallest=abs(den); }
    // loop over integers to find greatest common factor
    for (int i=1; i<=smallest; i++) {
      if ((num%(i+1)==0)&&(den%(i+1)==0)) { gcf=i+1; }
    } 
    num/=gcf; den/=gcf;
  }
  else {den=1;} // if the rational is zero, set the denominator to unity
}

// ******************************** MAIN ******************************** //
// ********************************************************************** //
int main() {

  // test the constructors
  Rational a(200,300); //a(2,3);
  Rational b(2);
  Rational c(-5,2);
  Rational d;
  Rational temp;
  
  // first print the rational numbers
  cout << "******* Testing Rational Numbers *******" << endl;
  cout << "a\t=\t"; a.print(); cout << "\t=\t"; a.nprint(); cout << endl;
  cout << "b\t=\t"; b.print(); cout << "\t=\t"; b.nprint(); cout << endl;
  cout << "c\t=\t"; c.print(); cout << "\t=\t"; c.nprint(); cout << endl;
  cout << "d\t=\t"; d.print(); cout << "\t=\t"; d.nprint(); cout << endl << endl;

  // then do some tricky operations and print results
  temp=a-2;
  cout << "a-2\t=\t"; temp.print(); cout << endl;
  temp=b+c;
  cout << "b+c\t=\t"; temp.print(); cout << endl;
  temp=c*a;
  cout << "c*a\t=\t"; temp.print(); cout << endl;
  temp=c/a;
  cout << "c/a\t=\t"; temp.print(); cout << endl;
  temp=a/c;
  cout << "a/c\t=\t"; temp.print(); cout << endl;
  temp=a/d;
  cout << "a/d\t=\t"; temp.print(); cout << endl;
  temp=b/15;
  cout << "b/15\t=\t"; temp.print(); cout << endl;
  temp=d/15;
  cout << "d/15\t=\t"; temp.print(); cout << endl;
  temp=c*a-b;
  cout << "c*a-b\t=\t"; temp.print(); cout << endl; 
  temp=c*(a-b);
  cout << "c*(a-b)\t=\t"; temp.print(); cout << endl << endl; 

  // finally compare some rational numbers
  cout << "a==c\t";
  if (a==c) { cout << "yes" << endl; }
  else { cout << "no" << endl; }
  cout << "b==d+2\t";
  if (b==(d+2)) { cout << "yes" << endl; }
  else { cout << "no" << endl; }
  cout << "d==0\t";
  if (d==0) { cout << "yes" << endl; }
  else { cout << "no" << endl; }
  
  return 1;
}
