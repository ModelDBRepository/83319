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



#include "network.h"


double log_e(double a){
	return(log(a)*2.30258509299);
}



double gauss(double m, double et){
double x1 = (((double)rand()+1.)/(double)(RAND_MAX+1));
double x2 = (((double)rand()+1.)/(double)(RAND_MAX+1));
//x1 , x2 != 0.!!
if (x1 < 0.) x1 = -1.*x1;
if (x2 < 0.) x2 = -1.*x2;


double y1 = sqrt(-2.*log_e(x1)) * cos(2*3.1416*x2);
double y2 = sqrt(-2.*log_e(x1)) * sin(2*3.1416*x2);
return(m+y2*et);
}


//return true 1 time per c times
bool proba(double c){
	return(std::rand()<=((double)RAND_MAX)/c);
}




/*--------------------------Network CLASS-------------------------------------*/


/*CONSTRUCTOR*/

Network::Network(int nb_neurons_){


#define IFSC_PREC 1e-4 
//To compute rho
IFSC_Init(Taui);



time_t timer1;
time(&timer1);

srand(localtime(&timer1)->tm_min + localtime(&timer1)->tm_hour+400*localtime(&timer1)->tm_sec);



	nb_neurons = nb_neurons_;
	q = new Ncq(SIM_LENGTH);
	table = (Neurone **)malloc(nb_neurons*sizeof(Neurone *));
	
	//initalization of neurons : 4/5 are inhibitor, 1/5 excitator.
	int i;
	for (i =0;i<INHIBITORY_NEURONS;i++){
		table[i] = new Neurone(table,q,gauss(-5.,5.),Vr_,Vt_,El_,Ee_,Ei_,gauss(4.,1.5), gauss(20.,12.),Taui,Taum,Dgi_,Dge_,true,i);
	}
	for (i =INHIBITORY_NEURONS;i<nb_neurons;i++){
		table[i] = new Neurone(table,q,gauss(-5.,5.),Vr_,Vt_,El_,Ee_,Ei_,gauss(4.,1.5), gauss(20.,12.),Taui,Taum,Dgi_,Dge_,false,i);
	}



//initialization of synapses
	for (i=0;i<nb_neurons;i++){
		int j;
		for (j=0;j<nb_neurons;j++){
			if (i!=j){
				if (proba(SYNAPSES_)) {  table[i]->voisins.push_back(table[j]);   }
			}
		}
	}
};




/*-----DESTRUCTOR-----*/
Network::~Network(){
	delete q;
	int i;
	for (i=0;i<nb_neurons;i++){
		delete table[i];
	}
	
	free(table);
IFSC_Done();

};






/*--------------Network::START()--------------------*/
void Network::start(){



double t = 0.;

while(t<=SIM_LENGTH && !(q->empty())) {
		
	Spike *s = q->pop(); //s = Next spike
	


#ifdef WITH_LOWER_BOUND
	if (!(s->computed)) { 
		table[s->sender]->upgrade();
	}

	else { //the time is exact
#endif //WITH_LOWER_BOUND
#ifndef NO_TERM
	printf("%d %f\n",s->sender,s->t*Taum); //id time
#endif //NO_TERM

	t = s->t;
	backup.push(*s);

(table[s->sender])->pulse(); //Pulse the "owner" of the spike


#ifdef WITH_LOWER_BOUND
	}
#endif


}

};


