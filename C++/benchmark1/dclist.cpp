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




/*Dclist is a data structure : a double chained list whith spikes*/


#include "dclist.h"	


Dclist::Dclist(){first = NULL;last = NULL;n=0;};



Spike *Dclist::pop(int day){
if (first == NULL) return NULL;
if (first->day != day) return NULL;
n--;
Spike *t = first;
first = t->next;

if (first == NULL) last = NULL;
else first->prev = NULL;
return(t);
};




void Dclist::remove(Spike *s){
n--;
Spike *p = s->prev;
Spike *n = s->next;

if (p!= NULL ) { p->next = n; 
				 if (n!= NULL ) { n->prev = p; }
				}
else //s = first 
	{ 
		first = n; 
		if (n != NULL) n->prev = NULL; 
	}

	if (last == s) { last = s->prev; 
		if (last != NULL) last->next = NULL;
	}

};





void Dclist::insert(Spike *s){


n++;
Spike *c = first;

if (c == NULL) { first = s; s->prev = s->next = NULL; last = s; }
	else {
		if (((c->t)>(s->t))&&(c->day>=s->day)) {  first = s; first->prev = NULL; first->next = c; first->next->prev = s; }
			else {
				while ((c->next != NULL ) && (c->next->day <= s->day ) && ( (c->next->t) <= (s->t)) ) {
				c = c->next;
}

//we insert s between c and c->next
s->next = c->next;
s->prev = c;

if (c->next != NULL ) c->next->prev = s;
c->next = s;
			}
if (s->next == NULL) last = s;
	}
};





bool Dclist::empty(){
	return (first == NULL);
};



int Dclist::number(){
	return(n);

}

