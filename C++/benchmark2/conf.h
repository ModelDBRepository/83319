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

//###########################CONFIGURATION########################################																	

//##########	NETWORK		#######
#define NEURONS 2000		//number of neurons		
#define LTH 1000.					//Length of the simulation in ms
#define AVERAGE_SYNAPSES 40		//Average number of synapses ( linked oriented neurons ) per neuron
#define INHIBITORY_PROPORTION 0.2	//Inhibitor neurons proportion, Warning !! : 0 <= INHIBITORY_PROPORTION <= 1 , type = double
//###################################


//#################  OUTPUT   ##################
		//Comment next line to see results in the terminal	
//#define NO_TERM
#define FILE_OUT "Network.log"  //OUTPUT FILE
//#############################################

//####################  Constants ##############
#define REFRACT_ 5.			//Length of the refract. Period , ms. 
#define Tau 20.				//Membrane time constant , ms
#define Taue_ 10.			//Excitatory time constant
#define Taui_ 5.			//Inhibitory time constant
#define Dge_ (0.6*60.)		//Excitatory synaptic weight
#define Dgi_ (6.7*(-20.))	//Inhibitory synaptic weight

//Be carrefull, you must write next values between ( ) 
#define El__ (-60.)		//Leak reversal potential, mV
#define Vt__ (-50.)		//Threshold  potential, mV
#define Vr__ (-60.)		//Reset potential, mV
#define Ee__ (0.)		//Excitatory reversal potential, mV
#define El__ (-60.)		//Leak reversal potential, mV
#define Ei__ (-80.)		//Inhibitory reversal potential, mV


//#############################################


//#################################################################################




//################For EXPERIMENTED USERS##########################################
#define AVERAGE_DAY_LENGTH (0.12) //Experimental value founded such as : in the queue, at less 80% of spikes must be in [t,t+AVERAGE_DAY_LENGTH] where t is a relative time
/*This option optimizes the computation time, but cannot modify the simulation*/
//##################################################################################





//################################## DON'T MODIFY NEXT LINES: #######################################"
#define INHIBITORY_NEURONS ((int)(NEURONS*INHIBITORY_PROPORTION))									
#define SYNAPSES_ (((double)NEURONS)/(double)AVERAGE_SYNAPSES)
#define EPS 0.00000001
#define REFRACT REFRACT_ //A VERIFIER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#define SIM_LENGTH LTH //Double

#define Taue Taue_
#define Taui Taui_
#define Vt_ (Vt__- El__)   
#define Vr_ (Vr__- El__ )	
#define Ee_ (Ee__-El__)	
#define El_ 0.	
#define Ei_ (Ei__-El__)	
//#####################################################################################################
