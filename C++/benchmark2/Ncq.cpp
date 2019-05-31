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



/*NCQ is a data structure ( in fact a calendar queue with double chained list ) which provide HERE a fast priority queue : 
NCQ is : -> a table with N buckets
		 -> each bucket is a double chained list

*/




#include "Ncq.h"
#include <math.h>






double ln2(double x){ return(log(x)/0.69314718055994530941723212145818);}


Ncq::Ncq(double t){
N = 200;							//# of buckets
M = AVERAGE_DAY_LENGTH ;					//duration of one day
W = M / (double)N;					//duration of one bucket
bucket = 0;							//current bucket
day = 0;							//current day
n = 0;								//number of elements
time = t;							//time of the simulation
int i;
for (i=0;i<N;i++) buckets.push_back(new Dclist());
};


Ncq::~Ncq(){
int i;
for (i = 0;i<N;i++) delete buckets[i];
};



void Ncq::insert(Spike *s){
	n++;
	insert_(s); 
};


void Ncq::insert_(Spike *s){
	
s->day = (int)(s->t/M);
buckets[(int)((s->t - ((double)s->day) * M) / W)]->insert(s);
}


bool Ncq::empty(){
return(n<=0);
};



Spike *Ncq::pop(){
if (n==0) return NULL;
Spike *r = buckets[bucket]->pop(day);
while (r == NULL){
	bucket++;
	if (bucket>=N) { bucket = 0; day++; }
	r = buckets[bucket]->pop(day);
}
n--;
return(r);
};


void Ncq::remove(Spike *s){
	if (s==NULL) return;

	(buckets[(int)((s->t - ((double)s->day) * M) / W)])->remove(s);
n--;
};


