#ifndef __CLASSIFIER__
#define __CLASSIFIER__

#include "../basic.h"
#include "../feature/libfeature.h"

#include "integralImage.h"

class Hypothesy{
public:	
	ulong _threshold;
	long _direction;
	double _alpha;
	FeatureMask _fm;

	Hypothesy(){};
	Hypothesy(ulong t, long d, double a, FeatureMask fm): _threshold(t), _direction(d), _alpha(a), _fm(fm){};
	
	ulong threshold(){return _threshold; };
	void threshold(ulong t){ _threshold = t; };

	long direction(){return _direction; };
	void direction(long d){ _direction = d; };	

	double alpha(){return _alpha;};
	void alpha(double a){ _alpha = a;};

	FeatureMask fm(){return _fm;};
	void fm(FeatureMask f){ _fm = f;};
};

class Classifier{
public:
	ulong _final;
	Point _ardis;
	double _ac;
	std::vector<Hypothesy> _hypothesis;

	Classifier(){};

	int isFace(IntegralImage &ii, Subwindow &sw, double ac);
	int isFace(IntegralImage &ii, double ac);
	int isFace(IntegralImage &ii, Subwindow &sw) {return isFace(ii,sw,_ac);};
	int isFace(IntegralImage &ii);

	inline ulong final(){return _final;};
	inline void final(ulong f){_final=f; };

	inline Point ardis(){return _ardis;};
	inline void ardis(Point p){_ardis=p;};

	inline std::vector<Hypothesy> hypothesis(){return _hypothesis;};
	inline void addHypothesy(Hypothesy &h){_hypothesis.push_back(h);};
};

inline int h_function(long filter, long direction, long threshold){
	if( (direction*filter)>(direction*threshold) ){
		return 0;
	}else{
		return 1;
	}
}

#endif