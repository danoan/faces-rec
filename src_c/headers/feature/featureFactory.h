#ifndef __FEATURE_FACTORY__
#define __FEATURE_FACTORY__

#define BLACK_PIXEL 0;
#define WHITE_PIXEL 1;
#define IGNORE_PIXEL 2;

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>

#include "../basic.h"
#include "featureMask.h"

/*
Given a start and end value, this function builds a list l of values such that
l[0] = first multiple of divisor after start or start
l[i] = first multiple of divisor after (l[i-1]*factor)

Example: 
>> increment_list(2,3,2,48)
>> [3,6,9,15,30] /*pivot [2,4,8,16,32]
*/
std::vector<ulong> incrementList(double factor, ulong divisor, ulong start, ulong end);

class FMF{
protected:
	Point _ardis;
	ulong _shift_w;
	ulong _shift_h;

	double _resize_factor;

	ulong _w;
	ulong _h;

	ulong _resize_w_step;
	ulong _resize_h_step;

	std::vector<Mask> _resizeList;
	std::vector<Point> _localList;

	Mask(* _fn_create)(Point);

	ulong _rit;
	ulong _lit;

public:
	FMF(){};
	FMF(Mask(* fn_create)(Point), Point,ulong,ulong,double,ulong,ulong,ulong,ulong);

	/*
	Resize the current template mask. For each horizontal stretch, it tries all the possible combinations
	of vertical strecthes.
	*/ 		
	virtual std::vector<Mask> resize(Mask(* fn_create)(Point));

	/*	
	Change the current local position on a Ardis accordinlgy with the current size of the mask and
	the size of the Ardis.
	*/			
	std::vector<Point> changeLocal(Point);
	std::vector<Point> changeLocal();
	FeatureMask next();

	inline int hasNext(){ return _rit>=this->_resizeList.size()?-1:1; };
	inline void restart(){ _rit=0; _lit=0; };
};

#endif