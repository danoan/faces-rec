#include <stdio.h>
#include <sys/time.h>

void main(){
	int i;
	double c;
	double ft;

	struct timeval stop,start;

	gettimeofday(&start,NULL);
	for(i=0;i<500000;i++){
		c = (i*3)/2.0;
	}
	gettimeofday(&stop,NULL);

	ft = (stop.tv_sec-start.tv_sec) + (stop.tv_usec-start.tv_usec)*1e-6;

	printf("%lf\n", ft);
}
