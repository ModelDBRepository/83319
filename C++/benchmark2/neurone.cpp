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



double gaussb(double m, double et){
double x1 = (((double)rand()+1.)/(double)(RAND_MAX+1) );
double x2 = (((double)rand()+1.)/ (double)(RAND_MAX+1));
//x1 , x2 != 0.!!
if (x1 < 0. ) x1 = -1.*x1;
if (x2 < 0. ) x2 = -1.*x2;

double y2 = sqrt(-2.*log(x1)) * sin(2*3.1416*x2);
return(m+y2*et);
}






//return the gcd between a and b
int pgcd(int a, int b){
	int r = a % b;
	if (r==0) return(b);
	return(pgcd(b,r));
};

//return the lcm of a and b
int ppcm(int a, int b){
	return((a*b)/(pgcd(a,b)));
};



//return the best int close to 'a' to avoid numerical error
int integer(double a){
	if ((a-(double)(int)a)< EPS) return ((int)a);
	return((int)(a+1));
};





//compute taulcm & compute the first spike
void Neurone::computeTaulcm(){
	taulcm = ppcm(ppcm(taui,taue),tau);
	tau3 = (int)(((double)taulcm)/taui);
	tau4 = (int)(((double)taulcm)/taue);
	spike = update_spike();
	if (spike != NULL) queue->insert(spike);

};





Neurone::~Neurone(){
delete spike;
};





Neurone::Neurone(Neurone **table_, Ncq *queue_, double Vinit, double Vrinit, double Vtinit ,double Iiinit, double Ieinit,double tauiinit, double taueinit,double tauinit,double dgiinit, double dgeinit, bool inhibiteur_, int id_){

	last_pulse = (-2.)*REFRACT;
	time = 0.;
	spike = NULL;
	queue = queue_;
	table = table_;
	inhibiteur = inhibiteur_;
	id = id_;
	V = Vinit;
	V0 = 0.;
	Vr = Vrinit;
	Vt = Vtinit;
	dge = dgeinit;
	dgi = dgiinit;
	taue = taueinit;
	taui = tauiinit;
	tau = tauinit;
	id = id_;
	Ie = Ieinit;
	Ii = Iiinit;
	

	tau1 = taui/(tau-taui);
	tau2 = taue/(tau-taue);


	if (V >= Vt) V = Vt - 1.;
	if (Ii< 0.) Ii *= -1.;
//	printf("(%d) V = %f\n",id,V);
	
};





void Neurone::ReceivePulse(int sender, double t){
	
	V = calcV(t-time);	//go to t
	updateI(t-time);		
	time = t;


	//receive the spike	
	if ((table[sender])->inhibiteur) {
		Ii += dgi; }
	else Ie += dge;


	//Test with new values if the neuron will spike
	Spike *newSpike = update_spike();


	if (spike != NULL) { queue->remove(spike); delete spike;}
	spike = newSpike;
	if (spike != NULL) queue->insert(spike);
	
};



//   /!\ called by the queue ( pop ), therefore the spike of the neuron isn't in the queue! /!\

void Neurone::pulse(){

//update state variable
	V = Vr;
	updateI(spike->t-time);
	time = spike->t;		
	last_pulse = time;
	
	
	//update next_spike
	delete spike;
	spike = update_spike();


if (spike != NULL)	queue->insert(spike);
	


	//the neuron spikes 
int i;
for (i=0;i<(int)voisins.size();i++)	{ voisins[i]->ReceivePulse(id,time); }

};




//return the new updated spike of the neuron
Spike *Neurone::update_spike(){

	/*
	tau1 = taui/(tau-taui)
	tau2 = taue/(tau-taue)
	tau3 = taulcm/taui
	tau4 = taulcm/taue
	*/


double reset_time;

double fi = 1;
double fe = 1;

if ((time-last_pulse)<REFRACT) { reset_time = REFRACT - ( time - last_pulse); fi = exp(-reset_time/taui); fe = exp(-reset_time/taue); }
if ((V - V0 + Ie*fe*tau2 + Ii*fi*tau1)< EPS) return NULL; //Descartes signs rule


	if ((time-last_pulse)<REFRACT){
		reset_time = REFRACT - ( time - last_pulse );


P.deg(0);
P.coeff(0,V0-Vt);
int c = integer((double)taulcm/tau);
double coeffc = Vr - V0;

coeffc += exp(-reset_time/taui)*Ii*tau1; 
coeffc += exp(-reset_time/taue)*Ie*tau2; 
Polynomial T(c,coeffc);
P = P + T;

	double coeffci = -Ii * exp(-reset_time/taui)*tau1; 
	int ci = tau3; 
	Polynomial R(ci,coeffci); 
	P = P + R; 
	

	double coeffce = -Ie * exp(-reset_time/taue)*tau2; 
	int ce = tau4; 
	Polynomial S(ce,coeffce); 
	P = P + S; }
	
	
	else { //CASE : time-last_pulse => REFRACT

reset_time = 0;
	

P.deg(0);
P.coeff(0,V0-Vt);

int c = ((double)taulcm)/tau;
double coeffc = V - V0;
coeffc += Ii*taui/(tau - taui); 
coeffc += Ie*taue/(tau - taue); 
Polynomial T(c,coeffc);
P = P + T;



	double coeffci = -Ii*tau1; 
	int ci = tau3; 
	Polynomial R(ci,coeffci);
	P = P + R; 
	

	double coeffce = -Ie *tau2; 
	int ce = tau4; 
	Polynomial S(ce,coeffce); 
	P = P + S; }


PolynomialSeq *sturmseq = (PolynomialSeq *)(P.sturmseq());
int r = P.sturm(sturmseq,0.,1.);
if (r<2) return NULL;

//ELSE
Spike *s = new Spike();
s->sender = id;
s->t = time  + reset_time - ((double)taulcm) * log(P.largestRoot(sturmseq,0.,1.));
return s;
	};






void Neurone::updateI(double newtime) //relative time!
{
Ii *= exp(-newtime/taui);
Ie *= exp(-newtime/taue);
};




 
double Neurone::calcV(double newtime){


if ((newtime+time) < (last_pulse + REFRACT)) { return Vr; }

if (time >= last_pulse + REFRACT) { 
	double expon = exp(-newtime/tau);
	double r;
	r = V0 + expon*(V - V0);
	
		r+=Ii*tau1*(-exp(-newtime/taui)+expon);
		r+=Ie*tau2*(-exp(-newtime/taue)+expon);

	return r;
}
	
	//ELSE
	

//Vtr = V(end of the REFRACT period)
		

	double reset_time = REFRACT - ( time - last_pulse );
	double active_time =	newtime - reset_time;
	
	double expon2 = exp(-active_time/tau);

	double r = V0 + ( Vr - V0 ) * expon2;
	
	
	r+=Ii*exp(-reset_time/taui)*tau1*(expon2 - exp(-active_time/taui));
	r+=Ie*exp(-reset_time/taue)*tau2*(expon2 - exp(-active_time/taue));


return(r);

};














