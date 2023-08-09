#include<stdio.h>
#include<time.h>

clock_t start, stop;
double duration;

int main ()
{
	double x=1.0001, result;
	int N;
	printf("Input N:\n");
	scanf("%d", &N);//input N
	
	start=clock();
	for(int j=0;j<1000;j++)//repeat 10^3 times
	{
		//re-initialize
		result=1;
		//compute x^N
		for(int i=0;i<N;i++){
			result*=x;
		}
	}
	stop=clock();

	duration=(double)(stop-start)/(double)CLOCKS_PER_SEC;//convert clock into seconds
	printf("Ticks:%d Seconds:%f\n",stop-start , duration);//output result
	
	return 0;
}