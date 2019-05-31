/*     ###########     Exact simulation of integrate-and-fire models with synaptic conductances ######################

This code is associated with the paper :
"Brette Romain (2006) , Exact simulation of integrate-and-fire models with synaptic conductances" :
http://www.di.ens.fr/~brette/papers/Brette2005NC.htm
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
#ifdef WITH_LOWER_BOUND	
			computed = true;
#endif //WITH_LOWER_BOUND

	};
	int sender;
	int day;
	double t; //the neuron 'from' will spike at 't' 

#ifdef WITH_LOWER_BOUND
	bool computed; //true if t is the exact spiking time, otherwise t is a lower bound of the spiking time
#endif //WITH_LOWER_BOUND


	Spike *next; //next and previous spike in the list, otherwise NULL
	Spike *prev;



	


};
