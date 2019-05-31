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



/*
characteristics of a neuron :
->a variable "spike" : if spike == NULL , the neuron will not spike, else it will spike at t = spike->t if he doesn't receive a pulse between [time,spike->t]
->a set of variable (V,g,Es) at t = time
*/


#include "neurone.h"



Neurone::~Neurone(){
delete spike;
};


Neurone::Neurone(Neurone **table_,Ncq *queue_,double Vinit, double Vrinit, double Vtinit,double Elinit,double Eeinit, double Eiinit, double geinit, double giinit, double tausinit, double tauminit ,double dgiinit, double dgeinit, bool inhibiteur_,int id_){
	V = Vinit;
	El = Elinit;
	
	ge = geinit;
	gi = giinit;
	Vr = Vrinit;
	Vt = Vtinit;
	Ee = Eeinit;
	Ei = Eiinit;
	dgi = dgiinit;
	dge = dgeinit;
	taus = tausinit;
	taum = tauminit;
	table = table_;
	inhibiteur = inhibiteur_;
	B = Vt - EPS;
	id = id_;
	queue = queue_;
	
	last_pulse = (-2.)*REFRACT;
	time = 0.;

	if (ge<0.) ge *= -1.;  //pour éviter d'avoir Es = +inf
	if (gi<0.) gi *= -1.;
	if (V >= Vt) { V = Vt - 0.5; }


	Es = (ge*Eeinit + gi*Eiinit)/(ge+gi);
	g = ge + gi;

	spike = update_spike(false);
	if (spike != NULL) { queue->insert(spike); }
};





void Neurone::ReceivePulse(int sender, double t){
	
	double gPulse;
		

//1)Update V
	double start_time = last_pulse + REFRACT; //end of the refract period
	double period = t - time; //length of the period since the last update

	if (time>=start_time){ V = calcV(period,V,g); }
	else { //time entre last_pulse et last_pulse + REFRACT  
		
		if ( t >= start_time ){ 
		
		double active_time =  t -  start_time ;	 
		V = calcV(active_time,Vr,g*exp(-(start_time-time)/taus));	


		}  //otherwise V = Vr...
	}  //otherwise V = Vr...



	//2)Compute g at t = "receiving the pulse"
	gPulse = g * exp(-(period)/taus); 
		


	//3) Receive the spike :
	if ((table[sender])->inhibiteur) { 
		//new parameters at t = ReceivePulse
		Es = (gPulse*Es + dgi * Ei)/(gPulse+dgi);
		gPulse += dgi;
	}

	else {
		Es = (gPulse*Es + dge*Ee)/(gPulse+dge);
		gPulse += dge;
	}


	
 //go to t and update g
	g = gPulse;
	time = t;


	//Test with new values if the neuron will spike
	Spike *newSpike = update_spike(0);

	if (spike != NULL) { queue->remove(spike); }
	spike = newSpike;

	if (spike != NULL ) 	queue->insert(spike);

	
};






//   /!\ called by the queue ( pop ), therefore the spike of the neuron isn't in the queue! /!\

void Neurone::pulse(){

//update state variable
	V = Vr;
	g *= exp(-(spike->t - time)/taus);
	time = spike->t;
	last_pulse = time;
	
	
	//update next_spike
	delete spike;
	spike = update_spike(0);


if (spike != NULL)	queue->insert(spike);	

	//the neuron spikes 
int i;
for (i=0;i<(int)voisins.size();i++)	{ voisins[i]->ReceivePulse(id,time); }


};









//return the new updated spike of the neuron
Spike *Neurone::update_spike(bool exact){

	double g_star;
	double V_g_star;


	if (V>=Vt){ Spike *s = new Spike(); s->sender = id; s->t = time ; return s; }

	g_star = Vt/(Es-Vt); // = 1./((Es/Vt)-1.);



if ((g<g_star) || (Es  < Vt )) { 
			return NULL;	
			}
	

	//ELSE

double t_begin; //relative time as soon as we start to look for the cross 
double gt; // = g if not in the REFRACT. period, otherwise  = g(end of the REFRACT period)
double st = time - last_pulse;



if (st>= REFRACT) { gt = g; t_begin = 0.;}
else { t_begin = REFRACT - st; gt = g*exp(-t_begin/taus); }


V_g_star = -taus*Es*g_star*rho(g_star)+pow(g_star/gt,taus)*exp(taus*(g_star-gt))*(V+taus*Es*gt*rho(gt));

if (V_g_star < Vt ) {
		return NULL;
}
//ELSE


//We can now compute the time for the next spike

#ifdef WITH_LOWER_BOUND
if (exact){
#endif //WITH_LOWER_BOUND

Spike *s = new Spike();
s->sender = id;
s->t = time + t_begin + findNextSpike(V,gt); 
return s;


#ifdef WITH_LOWER_BOUND
}



else { //exact = false
Spike *s = new Spike();
s->sender = id;
s->computed = false;
s->t = time + t_begin + lowerBound(V,gt); 
return s;
}
#endif //WITH_LOWER_BOUND

};



 


double Neurone::calcV(double newtime, double V0, double g0){

//gt = g0(newtime)
    double	gt = g0 * exp(-newtime/taus); 
	return(-taus*Es*gt*rho(gt)+exp(-newtime+taus*(gt-g0))*(V0+taus*Es*g0*rho(g0)));
}




double Neurone::findNextSpike(double V0, double g0){

double T = 0.;
double gT = g0;
double Vn = calcV(T,V0,g0);



while(Vn<B){
	
//Newton Raphson Method : Tn+1 = Tn - F(Tn)/F'(Tn)    with F(T) = calcV(T) - Vt
T += (Vt-Vn)/(-Vn+gT*(Es-Vn));
gT = g0 * exp(-T/taus);
Vn = calcV(T,V0,g0);

}

return T;
};




#ifdef WITH_LOWER_BOUND

//The same as findNextSpike() but with only few iterations for the newton raphson method
double Neurone::lowerBound(double V0, double g0){


double T = 0.;
double gT = g0;
int iter = 0;

double Vn = calcV(T,V0,g0);



while(iter<MAX_ITER){
	
//Newton Raphson Method : Tn+1 = Tn - F(Tn)/F'(Tn)    with F(T) = calcV(T) - Vt
T += (Vt-Vn)/(-Vn+gT*(Es-Vn));
gT = g0 * exp(-T/taus);
Vn = calcV(T,V0,g0);
iter++

}



return T;
};




void Neurone::upgrade(){

delete spike;
spike = update_spike(true);
if (spike != NULL) queue->insert(spike);
};
#endif //WITH_LOWER_BOUND



