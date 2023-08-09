#include<stdio.h>
#include<time.h>

clock_t start, stop;
double duration;

int main ()
{
	double x=1.0001, result, base;
	int N, temp;
	printf("Input N:\n");
	scanf("%d", &N);//input N
	
	start=clock();
	for(int i=0;i<1000000;i++){//repeate 10^6 times
		//re-initialize
		result=1;
		base=x;
		temp=N;
		//compute x^N by iterative algorithm
		while(temp>0){
			if(temp%2){
				result*=base;
			}
			base*=base;
			temp/=2;
		}
	}
	stop=clock();

	duration=(double)(stop-start)/(double)CLOCKS_PER_SEC;//convert clock into seconds
	printf("Ticks:%d Seconds:%f\n",stop-start , duration);//output result
	
	return 0;
}