
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
#include <vector>
#include "Ncq.h"
#include "polynomial.h"

using namespace std;





class Neurone
{
public:

	Spike *spike;
	vector<Neurone *> voisins;	

	bool etat;

	~Neurone();
	Neurone(Neurone **table_, Ncq *queue, double Vinit, double Vrinit, double Vtinit,double Iiinit, double Ieinit,double tauiinit ,double taueinit,double tauinit,double dgiinit, double dgeinit, bool inhibiteur_, int id);
	void ReceivePulse(int sender, double t);
	//bool update() ;
	//bool able_to_spike();
		void pulse();

	
	void computeTaulcm();
	




private:
	double V0;
	double taui,taue,tau, tau1, tau2;   // tau1 = taui/(tau-taui) , tau2 = taue/(tau-taue)
	int taulcm,tau3,tau4;				// tau3 = taulcm/taui , tau4 = taulcm/taue
	Polynomial P;
	double alpha, beta;
	int id; //id of the neuron ( in the table )
	double time;
	double V,Vt,Vr;
	double Ie,Ii,dge,dgi;
	Ncq *queue;
	Neurone **table; //table of neurons
	

	double last_pulse; //time since the last pulse
//Si le neurone est de type inhibiteur :
	bool inhibiteur; 
//Next spike of the neuron, otherwise NULL
 

	

	void updateI(double newtime); //for all Ii, Ii *= exp(-t/taui)
	double calcV(double newtime); //compute V at t = newtime if there is no spike
	Spike *update_spike();
	
	int ppcmList(int i, int j); //return the ppcm of Taui,Taui+1,...,Tauj

//	double findNextSpike(); //give the time of the next spike using newton-raphson method for (V(t) - Vt) 

};








