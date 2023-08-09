#include<stdio.h>
#include<time.h>

double POW (double x, int N);

clock_t start, stop;
double duration;

int main ()
{
	double x=1.0001, result, base;
	int N, temp;
	printf("Input N:\n");
	scanf("%d", &N);//input N
	
	start=clock();
	for(int i=0;i<1000000;i++){//repeat 10^6 times
		//compute x^n by recurtion
		result=POW(x, N);
	}
	stop=clock();

	duration=(double)(stop-start)/(double)CLOCKS_PER_SEC;//convert clock into seconds
	printf("Ticks:%d Seconds:%f\n",stop-start , duration);//output result
	
	return 0;
}

double POW (double x, int N)
{
	double result;
	//exit of recursion
	if(N==0) result=1;
	else if(N==1) result=x;
	//continue to recursion
	else{
		result=POW(x, N/2);
		result*=result;
		if(N%2) result*=x;
	}
	//return
	return result;
}