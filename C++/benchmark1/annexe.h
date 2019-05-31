#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#ifndef ANNEXE
#define ANNEXE

#define M_PI 3.1415926535897932385
#define IFSC_PREC 1e-4 






// Initialize (create the tables) and
//   set the constants:
//     taus = synaptic time constants (in units of the membrane time constant)
void IFSC_Init(double taus);

// Delete the tables
void IFSC_Done();



// *****************************************************************
// Exact simulation of integrate-and-fire models with
// exponential synaptic conductances
//
// Romain Brette
// brette@di.ens.fr
// Last updated: Jul 2005
// *****************************************************************
//
// Insert #define IFSC_PREC 1e-4 to use precalculated tables
// otherwise the original expressions are used
//
// All variables are normalized, i.e. V0=0, Vt=1, tau = 1





// Initialize (create the tables) and
//   set the constants:
//     taus = synaptic time constants (in units of the membrane time constant)
void IFSC_Init(double taus);

// Delete the tables
void IFSC_Done();


double tableRho(double g);
double rho(double g);




/*
double gamma(double x);


double incog(double a,double x);

double rho(double a, double x);*/
#endif //ANNEXE

