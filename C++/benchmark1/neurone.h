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



/*---------------NEURON CLASS-----------------------------*/

#include <vector>
#include "annexe.h"
#include "Ncq.h"


using namespace std;


class Neurone
{
public:
	Spike *spike; //Spike of the neuron. If no spike, spike = NULL
	vector<Neurone *> voisins;	//Synapses				

	//Constructor & destructor
	~Neurone();
	Neurone(Neurone **table_, Ncq *queue, double Vinit, double Vrinit, double Vtinit,double Elinit,double Eeinit, double Eiinit, double geinit, double giinit, double tausinit, double tauminit ,double dgiinit, double dgeinit, bool inhibiteur_, int id);
	/*Constructor :
		queue = Queue of spikes 
		Vinit = initial Potential 
		Vrinit = Reset Potential
		Vtinit = Threshold potential
		Elinit = Leak reversal potential
		Eeinit = Excitatory reversal potential
		Eiinit = Inhibitory reversal potential
		geinit = initial Excitatory conductance
		giinit = initial Inhibitory conductance
		tausinit = Synaptic time constant
		tauminit = Membrane time constant
		dgiinit = Inhibitory synaptic weight
		dgeinit = Excitatory synaptic weight
		inhibiteur_ = true if the neuron is inhibitor
		id_ = id of the neuron
	*/

	void ReceivePulse(int sender, double t); //Called if the neuron receive a spike
	void pulse(); //Called if the neuron must pulse
	
#ifdef WITH_LOWER_BOUND
	void upgrade(); //Call by the network if  we need the exact spiking time ( in fact when the "lower bound spike" is on the top of the priority queue )
#endif //WITH_LOWER_BOUND



private:
	int id; //id of the neuron ( in the table )
	double time; //Current time for the neuron
	double V,Vt,Vr; //V = Current potential
	double El,Es, Ee, Ei; //see the constructor
	double taus,taum;	
	double g,ge,gi;
	double dge,dgi;
	Ncq *queue;      //Queue of spikes
	Neurone **table; //table of neurons
	

	double last_pulse; //time since the last pulse
	bool inhibiteur; //true if inhibitor

 	double calcV(double newtime, double V0, double g0); //compute V at t = newtime ( relative time ) if there is no spike , and if at t = 0, V = V0, g = g0
	Spike *update_spike(bool exact);        //Return the next spike, otherwise NULL. if exact then we compute the exact spiking time, otherwise only a lowerbound

	double findNextSpike(double V0, double g0); //give the time of the next spike using newton-raphson method for (V(t) - Vt) if at t = 0, V = V0, g = g0, if there is no spike


#ifdef WITH_LOWER_BOUND
	double lowerBound(double V0, double g0); //give a lower bound of the time of the next spike using newton-raphson method for (V(t) - Vt) if at t = 0, V = V0, g = g0, if there is no spike, and with MAX_ITER iterations
#endif

	double B; //Bound for Newton Raphson Method
};








