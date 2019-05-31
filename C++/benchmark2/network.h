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






#include "neurone.h"
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

class Network{
public:
	Network(int nb_neurons);
	~Network();
	void start();
	queue<Spike> backup;
#ifdef STATS
Ncq *q;
#endif


private:
#ifndef STATS
	Ncq *q;
#endif
	int nb_neurons;
	Neurone **table;





};


