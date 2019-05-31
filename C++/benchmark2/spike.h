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



#include "conf.h"


#include <stdio.h>
#include <stdlib.h>
#include <iostream>


class Spike{
public:
	Spike(){
			next = NULL; 
			prev = NULL; 
	};
	int sender;
	int day;
	double t; //the neuron 'from' will spike at 't' 



	Spike *next; //next and previous spike in the list, otherwise NULL
	Spike *prev;



	


};
