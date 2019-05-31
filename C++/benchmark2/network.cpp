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
***********************************************************************************************************/


#include "network.h"





double gauss(double m, double et){
double x1 = ( ((double)rand()+1.)/(double)(RAND_MAX+1) );
double x2 = (((double)rand()+1.)/ (double)(RAND_MAX+1));
//x1 , x2 != 0.!!
if (x1 < 0.) x1 = -1.*x1;
if (x2 < 0.) x2 = -1.*x2;

double y2 = sqrt(-2.*log(x1)) * sin(2*3.1416*x2);
return(m+y2*et);
}

bool proba(double c){
	return(std::rand()<=((double)RAND_MAX)/c);
}




Network::Network(int nb_neurons_){



time_t timer1;
time(&timer1);


	nb_neurons = nb_neurons_;
	q = new Ncq(SIM_LENGTH);
	table = (Neurone **)malloc(nb_neurons*sizeof(Neurone *));
	
	//initalisation
	int i;
	for (i =0;i<INHIBITORY_NEURONS;i++){
		table[i] = new Neurone(table,q,gauss(-5.,5.),Vr_,Vt_,gauss(20.,12.),gauss(4.,1.5),Taui,Taue,Tau,Dgi_,Dge_,true,i);		
	}
	for (i =INHIBITORY_NEURONS;i<nb_neurons;i++){
		table[i] = new Neurone(table,q,gauss(-5.,5.),Vr_,Vt_,gauss(20.,12.),gauss(4.,1.5),Taui,Taue,Tau,Dgi_,Dge_,false,i);
	
	}
	
	
	//Synapses initialization
	for (i=0;i<nb_neurons;i++){

	
		int j;
		for (j=0;j<nb_neurons;j++){
			if (i!=j){
				if (proba(SYNAPSES_)) { 		
							(table[i]->voisins).push_back(table[j]);   
				}
			}
		}
	}

//computation of taulcm, and end of initalization
	for (i=0;i<nb_neurons;i++){
		table[i]->computeTaulcm();
	}
};




Network::~Network(){
	delete q;
	int i;
	for (i=0;i<nb_neurons;i++){
		delete table[i];
	}
	


};





void Network::start(){

double t = 0.;
	
while(t<=SIM_LENGTH && !(q->empty())) {
	
	Spike *s = q->pop();
	t = s->t;

#ifndef NO_TERM
	printf("%d %f\n",s->sender,s->t);
#endif //NO_TERM
	(table[s->sender])->pulse();


	backup.push(*s);
}

};
