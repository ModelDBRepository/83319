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



#include "spike.h"



/*-------------------Dclist = double chained list with spikes--------------------*/

class Dclist{
public:
	Dclist();; //constructor
	Spike *pop(int n);		//return the first spike of the list if its day is n
	void remove(Spike *s);  //remove the spike s
	void insert(Spike *s);	//insert the spike s
	bool empty();			//return true if the list is empty, otherwise false
	
	//For stats : 
	int number();			//return the number of elements in the list


	


private:
	Spike *last;	//last spike of the list
	Spike *first;	//first spike of the list
	int n;			//number of events in the list

};


