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



double dabs(double a){ 
if (a<0.) return(-1.*a);
else return(a);
}


/*-----------Polynomial CLASS--------------*/

#include "polynomial.h"



#define EPS 0.000001

double min(double a, double b){
	if (a<b) return a;
	return b;
};


Polynomial::Polynomial(){
	n = 0;
	P.resize(1);
	P[0] = 0.;
};





Polynomial::Polynomial(int d){
	n = d;
	P.resize(n+1);
	int i;
	for (i=0;i<n;i++){ P[i] = 0.; }
	P[n] = 1.;
};


Polynomial::Polynomial(int d, double ad){
	n = d;
	P.resize(n+1);
	int i;
	for (i=0;i<n;i++){ P[i] = 0.; }
	P[n] = ad;
};



bool Polynomial::empty(){
return((n==0)&&(P[0]==0.));
};


void Polynomial::normalize(){
	if (n>0){
		if (P[n]==0.) { deg(deg()-1); normalize(); }
	}
};

void Polynomial::coeff(int c, double ac){
	if (c>n)  deg(c); 
	P[c] = ac;
	if (dabs(P[c])<EPS) { P[c] = 0.; }
	normalize();
};

double Polynomial::coeff(int c){
	if (c>n) { return 0.; }
	return(P[c]);
};


int Polynomial::deg(){
return(n);
};


void Polynomial::deg(int p){
	if (p>0){ P.resize(p+1);}
	if (p>n){
		int i;
		for (i=n+1;i<=p;i++) P[i] = 1.;
	}
n = p;

normalize();
}


double Polynomial::eval(double x){
int i;
double s = 0;
for (i=0;i<=n;i++){
	if (P[i]!= 0.) { s+= (P[i]*pow(x,i)); }
}
return s;
};

void Polynomial::view(){
int i;
for (i=n;i>=0;i--){
	if (P[i] != 0.){printf(" + %f X^%d",P[i],i);}
}
	printf("\n");
};



Polynomial Polynomial::operator+(Polynomial &Q){
Polynomial R;
R.deg(deg()+Q.deg());
int i;
for(i=0;i<=R.deg();i++){ R.coeff(i,Q.coeff(i)+coeff(i));}

//normalization
int max = R.deg();
while (R.coeff(max) == 0. ){ max --;}
R.deg(max);
return R;
};


Polynomial Polynomial::operator+(double q){
Polynomial R(deg());
int i;
for (i=0;i<=deg();i++){ R.coeff(i,coeff(i)+q); }
return R;
}



Polynomial Polynomial::operator*(Polynomial &Q){
int degd = deg() + Q.deg();
Polynomial R;
R.deg(degd);
int i,d;

for (d=0;d<=degd;d++){
	double s = 0.;

	for (i=0;i<=d;i++) s+=coeff(i)*Q.coeff(d-i);
		R.coeff(d,s);
}
return R;
};





Polynomial Polynomial::operator*(double q){
Polynomial R;
R.deg(n);
int i;
for (i=0;i<=n;i++){
	R.coeff(i,q*coeff(i));
}
return R;
};




Polynomial Polynomial::operator-(Polynomial &Q){
int mind = min(this->deg(),Q.deg());
Polynomial R;
R.deg(deg()+Q.deg());
int i;
for(i=0;i<=R.deg();i++){ R.coeff(i,coeff(i)-Q.coeff(i));}

//normalization
int max = R.deg();
while (R.coeff(max) == 0. ){ max --;}
R.deg(max);
return R;
};










Polynomial Polynomial::operator/(Polynomial &Q){
Polynomial R;
R.coeff(0,0.);
if (deg()<Q.deg()) return R;

//ELSE
R.deg(deg()-Q.deg());

Polynomial S = (*this);

while (S.deg()>=Q.deg() || S.empty()){
Polynomial T;
T.coeff(0,0);
T.deg(S.deg()-Q.deg());
T.coeff(T.deg(),S.coeff(S.deg())/Q.coeff(Q.deg()));
R=R+T;
S=S-(T*Q);
}

return R;
};





Polynomial Polynomial::operator%(Polynomial &Q){


Polynomial R;
R.coeff(0,0.);
if (deg()<Q.deg()) return R;

//ELSE
R.deg(deg()-Q.deg());

Polynomial S = (*this);

while (S.deg()>=Q.deg()){

Polynomial T;
T.coeff(0,0);
T.deg(S.deg()-Q.deg()); 
T.coeff(T.deg(),S.coeff(S.deg())/Q.coeff(Q.deg()));
R=R+T;

//to avoid problems with numerical approximation 
int d = S.deg();
S=(S-(T*Q));
if (S.deg() == d) { S.deg(d-1); }
}

return S;
};










bool Polynomial::operator==(Polynomial &Q){
	int d = Q.deg();
	if (d!=deg()) return false;
	int i;
	for (i=0;i<=d;i++){
		if (Q.coeff(i) != coeff(i)) return false;
	}
	return true;
};






void Polynomial::operator=(Polynomial Q){
int d = Q.deg();
deg(d);
int i;
for (i=0;i<=d;i++){coeff(i,Q.coeff(i)); }
};



int Polynomial::descartes(){
int i;
int s = 0;
for (i=0;i<n;i++){
	if (P[i]*P[i+1] < 0.) s++;
}
return s;
};


Polynomial Polynomial::deriv(){
Polynomial R = (*this);
int i;
for (i=1;i<=n;i++){
R.coeff(i-1,i*coeff(i));
}
R.deg(n-1);
return R;
};



void *Polynomial::sturmseq(){
	
PolynomialSeq *sturmseq = new PolynomialSeq();

Polynomial P0, P1,Pn;
P0 = (*this);
P1 = this->deriv();

Pn = (P0 % P1)*(-1.);
sturmseq->suite.push_back(P0);
sturmseq->suite.push_back(P1);
sturmseq->n = 2;

while (Pn.deg() >0 ){

sturmseq->suite.push_back(Pn);
sturmseq->n++;
P0 = P1;
P1 = Pn;

Pn = (P0 % P1)*(-1.);


}
sturmseq->suite.push_back(Pn);
sturmseq->n++;
return((void *)sturmseq);
};






int Polynomial::sturm(void *s_,double u, double v){
int i;

PolynomialSeq *s = (PolynomialSeq *)s_;

//We compute Card{i,Pi(u)Pi+1(u)<0} - Card{i,Pi(v)Pi+1(v)<0} 
int cardu = 0;
int cardv = 0;

 
for (i=0;i<s->n;i++){ if (((s->suite[i]).eval(u) * s->suite[i+1].eval(u))<0.) cardu++; }
for (i=0;i<s->n;i++){ if (((s->suite[i]).eval(v) * s->suite[i+1].eval(v))<0.) cardv++; }


return(cardu-cardv);
};






double Polynomial::largestRoot(void *s_, double u, double v){


PolynomialSeq *s = (PolynomialSeq *)s_;

//We compute Card{i,Pi(u)Pi+1(u)<0} - Card{i,Pi(v)Pi+1(v)<0} 
int cardu = 0;
int cardv = 0;
int i; 
for (i=0;i<s->n;i++){ if (((s->suite[i]).eval(u) * s->suite[i+1].eval(u))<0.) cardu++; }
for (i=0;i<s->n;i++){ if (((s->suite[i]).eval(v) * s->suite[i+1].eval(v))<0.) cardv++; }


int cardm;

double m = (u+v)/2.;
while ((v-u)> EPS) { 
	cardm = 0;
	for (i=0;i<s->n;i++){ if (((s->suite[i]).eval(m) * s->suite[i+1].eval(m))<0.) cardm++; }
	if ((cardm - cardv) > 0) { /*printf("u = m\n");*/ cardu = cardm; u = m;}
	else {  v = m ; cardv = cardm;  }
	m = (u+v)/2.;
}
return(u);
};

