// A problem from Dr. Duez 6.14.10
// Brett Deaton

#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<math.h>
#include<iostream>
using namespace std;

void setup_gaussian(int N, double lambda,
					double xl, double xu,
					vector<double>& func)
{
	//  set up so func[i] = exp(-(x[i]*x[i])/(lambda*lambda))
	//  where x[0] = xl, x[N-1] = xu
	//  with equal increments between elements: x[2]-x[1] = x[3]-x[2], etc
	double dx = (xu-xl)/(N-1);
	vector<double> x(N);
	// populate an ordinal vector
	for (int i=0; i<N; i++)
		x[i] = xl + i*dx;
	for (int i=0; i<N; i++)
		func[i] = exp(-(x[i]*x[i])/(lambda*lambda));
}

double sum(vector<double> func) {
	//  sum over all points in the vector
	//  return the sum
	double s = 0;
	for (int i=0; i<func.size(); i++)
		s+=func[i];
	return s;
}

vector<double> vector_sum (vector<double> func1,
						   vector<double> func2) {
	//  First check to make sure that func1 and func2
	//  really do have equal length.  If they don't,
	//  print a warning.
	vector<double> vsum(func1.size());
	if (func1.size()!=func2.size()) {
		cout << "Error: your input vectors are different lengths."
		<< " No sum was computed."
		<< endl;
	}
	//  Calculate another vector of the same length
	//  with entries func1[i]+func2[i].
	for (int i=0; i<func1.size(); i++) {
		vsum[i] = func1[i]+func2[i];
	}
	return vsum;
}

int main() {
	// allocate memory for a vector func1
	int N = 1000;
 	vector<double> func1(N);
 	
	// initialize func1 with the function
	// exp(-(x*x)/(lambda*labmda))
	// at N equally spaced gridpoints between
	// x=xl and x=xu
	double xl = -6.0, xu = 6.0;
 	double lambda1 = 1.0;
	setup_gaussian(N,lambda1,xl,xu,func1); 	
	
	// sum the function:
	// sum(f) = f[0] + f[1] + ... + f[N-1]
 	double sum_f1 = sum(func1);
	cout << "sum_f1 = "
	<< sum_f1 << endl;
	
	// make another function 	
    vector<double> func2(N);
 	double lambda2 = 2.0;
	setup_gaussian(N,lambda2,xl,xu,func2); 
	
	double sum_f2 = sum(func2);
	cout << "sum_f2 = "
	<< sum_f2 << endl;
	
	// sum func1 and func2
	// f1_plus_f2[i] = f1[i] + f2[i]
	vector<double> f1_plus_f2
	= vector_sum(func1,func2);
	
	// sum vector_sum	
 	double sum_sum = sum(f1_plus_f2);
	cout << "sum_sum=" << sum_sum
	<< endl;
	
	// check
	cout << "sum(f1+f2) - sum(f1) - sum(f2) = "
	<< sum_sum - sum_f1 - sum_f2 << endl;
	
 	return 1;	
}

// output sums should be:
// sum_f1 = 147.557
// sum_f2 = 295.107
// sum_sum=442.664
