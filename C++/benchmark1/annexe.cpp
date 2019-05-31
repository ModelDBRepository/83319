#include "annexe.h"


// *****************************************************************
// Exact simulation of integrate-and-fire models with
// exponential synaptic conductances
//
// Romain Brette
// brette@di.ens.fr
// Last updated: Jul 2005
// *****************************************************************
//
// Insert #define IFSC_PREC 1e-4 in file IFSC.h to use precalculated tables
// otherwise the original expressions are used
//
// All variables are normalized, i.e. V0=0, Vt=1, tau = 1
//
// N.B.: the code is not optimized



// ************************************************************
// CONSTANTS
// ************************************************************

// The neuron spikes when the membrane potential
// is within SPIKE_COMPUTATION_ERROR of the threshold



#ifdef IFSC_PREC
#define SPIKE_COMPUTATION_ERROR	1e-10
#else
#define SPIKE_COMPUTATION_ERROR	1e-6
#endif

// Relative error in calculating the incomplete Gamma integral
#define MAX_GAMMA_ERROR 1e-10

// Time constant of the synaptic conductances and its inverse
// (taus_ is in units of the membrane time constant)
double taus_;
double invtaus_;

double rho(double g);

#ifdef IFSC_PREC
// ************************************************************
// LOOK-UP TABLES
// ************************************************************
// 
// The values are computed with the look-up tables 
// using linear interpolation as follows:
//
// cell number:     n=(int)(x/dx)    (dx is the precision of the table)
// remainder:       h=(x/dx)-n       (for the linear interpolation)
// value:           y=table[n]+h*(table[n+1]-table[n])
//
// ----------------------------------------------------------
//    Look-up table for the exponential (exp(-x))
// ----------------------------------------------------------
//
// Address
double *expLookupTable;
// Precision
double expLookup_dx;
// Inverse of precision
double inv_expLookup_dx;
// Size
int expLookup_nmax;
// 1./(taus_*expLookup_dx)
double invtaus__X_inv_expLookup_dx;

// --------------------------------------------------
//    Look-up table for the (modified) rho function
//       rho(1-\tau_s,\tau_s g) * g * \tau_s
// --------------------------------------------------
// Address
double *rhoLookupTable;
// Precision
double rhoLookup_dg;
// Inverse of precision
double inv_rhoLookup_dg;
// Size
int rhoLookup_nmax;

// -----------------------------------------------
// Functions for the exponential look-up table
// -----------------------------------------------
//
// Build the look-up table for x in [0,xmax] with precision dx
void makeExpLookupTable(double dx,double xmax) {
	double x;
	int n;

	expLookup_dx=dx;
	inv_expLookup_dx=1./dx;
	invtaus__X_inv_expLookup_dx=invtaus_*inv_expLookup_dx;
	expLookup_nmax=(int)(xmax*inv_expLookup_dx);

	expLookupTable=(double *)
		malloc(sizeof(double)*(expLookup_nmax));

	x=0.;
	for(n=0;n<expLookup_nmax;n++) {
		expLookupTable[n]=exp(-x);
		x+=dx;
	}

	expLookup_nmax--;
}

// Free the memory allocated for the table
void freeExpLookupTable() {
	free((void *)expLookupTable);
}

// tableExpM(x) returns exp(-x) from the look-up table
// (with linear interpolation)
double tableExpM(double x) {
	double a,b;
	double *table;
	int n=(int)(x=x*inv_expLookup_dx);

	if ((n>=expLookup_nmax) || (n<0)) {
		return (exp(-x*expLookup_dx));
	}

	table=expLookupTable+n;
	a=*(table++);
	b=*table;

	return ( a + (x-n)*(b - a) );
}

// -----------------------------------------------
// Functions for the rho look-up table
// -----------------------------------------------
//
// Build the look-up table for g in [0,gmax] with precision dg
void makeRhoLookupTable(double dg,double gmax) {
	double g;
	int n;

	rhoLookup_dg=dg;
	inv_rhoLookup_dg=1./dg;
	rhoLookup_nmax=(int)(gmax/dg);

	rhoLookupTable=(double *)
		malloc(sizeof(double)*(rhoLookup_nmax));

	g=0.;
	for(n=0;n<rhoLookup_nmax;n++) {
		rhoLookupTable[n]=rho(g)*g*taus_;
		g+=dg;
	}

	rhoLookup_nmax--;
}

// Free the memory allocated for the table
void freeRhoLookupTable() {
	free((void *)rhoLookupTable);
}

// tableRho(g) returns rho(g) from the look-up table
// (with linear interpolation)
double tableRho(double g) {
	double a,b;
	double *table;
	int n=(int)(g=g*inv_rhoLookup_dg);

	if ((n>=rhoLookup_nmax) || (n<0)) {
		return (rho(g*expLookup_dx));
	}

	table=rhoLookupTable+n;
	a=*(table++);
	b=*table;

	return ( a + (g-n)*(b - a) );
}

#endif

// **************************
// CONSTRUCTION & DESTRUCTION
// **************************

// Initialize (create the tables) and
//   set the constants:
//     taus = synaptic time constants (in units of the membrane time constant)
void IFSC_Init(double taus){
	taus_=taus;
	invtaus_=1./taus_;

	

	makeRhoLookupTable(IFSC_PREC,1.);
	
}

// Delete the tables
void IFSC_Done() {

	freeRhoLookupTable();
	

}

// ************************************************************
// RHO FUNCTION (based on incomplete gamma integral - see text)
// ************************************************************
// rho(g) = \rho(1-\tau_s,\tau_s * g)
// (see text)
//
// We use the power series expansion of the incomplete gamma integral
double rho(double g) {
	//printf("Rho appele avec %f\n",g);
    double sum, del, ap;
	double x=taus_*g;

	// Note: all numbers are always positive
	ap = 1.-taus_;
    del = sum = 1.0 / (1.-taus_);
	do {
		++ap;
        del *= x / ap;
        sum += del;

	
    } while (del >= sum * MAX_GAMMA_ERROR);
//printf("Sortie de rho\n");
	return sum;
}

