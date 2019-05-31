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


/*---------------Priority queue based on a Calendar Queue with modulo ( or with days )--------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "dclist.h"


using namespace std;






class Ncq{
	public:
	vector<Dclist *> buckets;	//Vector of Double chained list ( Dclist )
		Ncq(double  time);		//time = total time of the queue
		~Ncq();
Spike *pop(); 
void insert(Spike *s);
void remove(Spike *s);
bool empty();

int n;		//number of elements
int day;	//day of the list
int bucket;	//current bucket

private:
	
	void insert_(Spike *s); //insert without change of the length of the vector

	double M;	//duration of a day
	int N;		//number of buckets
	double W;	//width of a bucket
	double time;//total time of the list


};




