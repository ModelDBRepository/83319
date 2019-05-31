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


You can ( and must ) configure the network in the file conf.h

*/


/*
Syntax of output : "id of the neuron" "time of spike"

To study the phenomen, the spike are stored in n->backup in the right order, with time of spike in Taum unit ( only in the table, not in the file, not in the terminal )*/



#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include "network.h"










int main(int argc, char *argch){

int h2,m2,s2,h3,m3,s3;
time_t timer1;

printf("Loading...\n");		
time(&timer1);
srand(localtime(&timer1)->tm_min + localtime(&timer1)->tm_hour+400*localtime(&timer1)->tm_sec);
	

//Load the network 
Network *n = new Network(NEURONS);

//Compute the beginning time
time(&timer1);
h2 = localtime(&timer1)->tm_hour;
m2 = localtime(&timer1)->tm_min;
s2 = localtime(&timer1)->tm_sec;


//Start the network
printf("Starting!\n");
n->start();

//""
time(&timer1);
h3 = localtime(&timer1)->tm_hour;
m3 = localtime(&timer1)->tm_min;
s3 = localtime(&timer1)->tm_sec;

printf("# of Neurons : %d,# of Spikes : %d\n",NEURONS,n->backup.size());

printf("Length of the simulation : %d sec\n",(h3-h2)*3600+(m3-m2)*60+s3-s2);


//Write in the file if OUT is defined
#ifdef OUT
printf("Writing in %s...",FILE_OUT);		
FILE *file = fopen(FILE_OUT,"w");

int k = 0;
char buf[20];
while (!(n->backup.empty())){
	k++;
	Spike s = n->backup.front();
	n->backup.pop();
	sprintf(buf,"%d",(s.sender));
	fputs(buf,file);
	sprintf(buf,"%f",s.t*Taum);
	fputs(" ",file);
	fputs(buf,file);
	fputs("\n",file);
}
fclose(file);
printf("OK!\n");
#endif //OUT


delete n;
return 0;
}



