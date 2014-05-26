#include "../../headers/classifier/classifier.h"

int Classifier::isFace(IntegralImage &ii, Subwindow &sw, double ac){
	ulong t;
	long d;
	double a;
	FeatureMask fm;

	double sx=0;
	double sa=0;

	Hypothesy h;
	for(register int i=0;i<_hypothesis.size();++i){
		h = _hypothesis[i];

		t=h._threshold;
		d=h._direction;
		a=h._alpha;
		fm=h._fm;

		fm.adjustMask(sw);
		sx+= a*h_function(ii.filter(fm),d,t);		

		// printf("%d %lf %lu %ld %lu\n",h_function(ii.filter(fm),d,t), a,ii.filter(fm),d,t);
		sa+= a;
	}

	// printf("%.11f %lu %lu (%lu %lu)\n", sx, sw._y,sw._x,sw._size.y,sw._size.x);
	if(sx>=(ac*sa)){
		return 1;
	}else{
		return 0;
	}
}