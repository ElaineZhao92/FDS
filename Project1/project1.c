/*
author: Elaine Zhao
*/

#include<stdio.h>
#include<time.h>
clock_t start,stop;
double duration1,duration2,duration3;
double algorithm1(double X,double N);
double algorithm2_Iterative(double X,double N);
double algorithm2_Recursive(double X,double N);
int main()
{
	int j,K,N;
	double X;     //K is used to count the iteration times.
	start=clock();
	printf("Please write in the value of variable X, the coefficient N.\n");
	printf("INPUT:");
	scanf("%f %d",&X,&N);
    if(N>=1000&&N<=5000)    K=10000;
    else if(N>5000&&N<=60000)    K=1000;
    else if(N>60000&&N<=100000)  K=500;
	for(j=0;j<10000;j++)
	{
		algorithm1(X,N);
	}
	stop=clock();
	duration1=((double)(stop-start))/CLK_TCK/K;
	//Load the duration of algorithm 1.
	
	start=clock();
	for(j=0;j<10000;j++)
	{
		algorithm2_Iterative(X,N);
	}
	stop=clock();
	duration2=((double)(stop-start))/CLK_TCK/K;
	//Load the duration of algorithm2_Iterative.
	
	start=clock();
	for(j=0;j<10000;j++)
	{
		algorithm2_Recursive(X,N);
	}
	stop=clock();
	duration3=((double)(stop-start))/CLK_TCK/K;
	//Load the duration of algotithm2_Recursive. 
	
	//Print the results.
	printf("%.10f\n",duration1);
	printf("%.10f\n",duration2);
	printf("%.10f\n",duration3);
	return 0;
} 

double algorithm1(double X,double N)
{
	double result=1;
	int i;
	for(i=0;i<=N-1;i++)	//it will be executed in N loops 
		result=result*X;
	return result;
}

double algorithm2_Iterative(double X,double N)
{
	double result=1;
	if(N==0) return 1;
	
	while(N>0)
	{
		if((int)N%2==1)	//it will be executed in logN loops
			result=result*X;//if N is odd
		X=X*X;//whenever N is odd or even
		N=N/2;
	}
	return result;
}

double algorithm2_Recursive(double X,double N)
{
	double result=1;
	if(N==0) return 1;
	else if(N==1) return X;
	else if((int)N%2==0) return algorithm2_Recursive(X*X,N/2);//even
	else if((int)N%2==1) return algorithm2_Recursive(X*X,N/2)*X;//odd
	
}
