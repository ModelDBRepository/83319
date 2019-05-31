/*     ###########  Exact simulation of integrate-and-fire models with exponential currents  ######################

This code is associated with the paper :
"Brette Romain (2006) ,  Exact simulation of integrate-and-fire models with exponential currents " :
http://www.di.ens.fr/~brette/papers/Brette2006NC.htm
*************************
*************************
*************************
Cohen Benjamin
benjamin.cohen _AT_ ens-lyon.fr
Last updated : Jul 2006 

If you modify the source file, please don't delete this header
***********************************************************************************************************
*/


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;





class Polynomial{

public:





	Polynomial(int d); //create the polynomial X^d
	Polynomial(int d, double ad); //create the polynomial ad*X^d
	Polynomial();
	bool empty();
	void coeff(int c, double ac); //coeff a_c <- ac
	double coeff(int c); //return a_c
	int deg(); //degree
	void deg(int n); //change the degree ( troncature )
	Polynomial operator+(Polynomial &);
	Polynomial operator+(double q);
	Polynomial operator*(Polynomial &);
	Polynomial operator*(double q);
	Polynomial operator-(Polynomial &);
	Polynomial operator/(Polynomial &);
	Polynomial operator%(Polynomial &);
	void operator=(Polynomial P);
	bool operator==(Polynomial &);
	double eval(double x);

	Polynomial deriv(); //return the derivate Polynomial
	int descartes(); //return M >= Card{ k , ak*a(k+1) < 0 }
	void *sturmseq();	//return the Sturm sequence of the Polynomial, the return type is (void *) because ::Polynomial doesn't know PolynomialSeq
	int sturm(void *s,double u, double v); //return the number of root in [u,v]
	double largestRoot(void *s, double u, double v); //give the largest root in [u,v] using a simple bissetion method

	void view(); //print the polynomial





private:
	int n; //degree of P
	std::vector<double> P;
	void normalize();
	
};





class PolynomialSeq{
public:
	PolynomialSeq(){n = 0;};
	int n;	//number of polynomials in the sequence
	vector<Polynomial> suite;
};

