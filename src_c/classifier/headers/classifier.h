#ifndef __CLASSIFIER__
#define __CLASSIFIER__

#include "../../util/headers/basic.h"
#include "../../feature/headers/libfeature.h"

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

class ClassifierInterface{
public:
	virtual int isFace(IntegralImage &ii, Subwindow &sw, double ac) = 0;
	virtual int isFace(IntegralImage &ii, double ac) = 0;
	virtual int isFace(IntegralImage &ii, Subwindow &sw) = 0;
	virtual int isFace(IntegralImage &ii) = 0;	
};

class Classifier:public ClassifierInterface{
public:
	Point _ardis;
	double _ac;
	std::vector<Hypothesy> _hypothesis;

	Classifier(){};

	int isFace(IntegralImage &ii, Subwindow &sw, double ac);
	int isFace(IntegralImage &ii, double ac);
	int isFace(IntegralImage &ii, Subwindow &sw);
	int isFace(IntegralImage &ii);

	inline Point ardis(){return _ardis;};
	inline void ardis(Point p){_ardis=p;};

	inline std::vector<Hypothesy> hypothesis(){return _hypothesis;};
	inline void addHypothesy(Hypothesy &h){_hypothesis.push_back(h);};

};

std::ostream& operator<<(std::ostream& os, const Hypothesy& h);
std::istream& operator>>(std::istream& is, Hypothesy& h);

std::ostream& operator<<(std::ostream& os, const Classifier& c);
std::istream& operator>>(std::istream& is, Classifier& c);

inline int h_function(long filter, long direction, long threshold){
	if( (direction*filter)>(direction*threshold) ){
		return 0;
	}else{
		return 1;
	}
}

#endif