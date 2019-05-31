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

	void start();			//Start the simulation
	queue<Spike> backup;	//Backup of spikes
	Ncq *q;					//Queue to provide exact simulation



private:
	int nb_neurons;		//number of neurons
	Neurone **table;	//vector of neurons
};


