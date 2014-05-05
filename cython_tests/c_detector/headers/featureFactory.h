#ifndef _FEATURE_FACTORY
#define _FEATURE_FACTORY

#define BLACK_PIXEL 0;
#define WHITE_PIXEL 1;
#define IGNORE_PIXEL 2;

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>

#include "basic.h"
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

	inline int hasNext(){ return _rit==this->_resizeList.size()?0:1; };
};

/*
0011
0011
0011

It always has even width.
*/
class MaskTwoHorizontalFactory: public FMF{
public:
	MaskTwoHorizontalFactory(Point ardis,ulong shift_w=1,ulong shift_h=1,double resize_factor=1.0,ulong w=2,ulong h=2);
};


/*
0000
0000
1111
1111

It always has even height.
*/
class MaskTwoVerticalFactory: public FMF{
public:
	MaskTwoVerticalFactory(Point ardis,ulong shift_w=1,ulong shift_h=1,double resize_factor=1.0,ulong w=2,ulong h=2);
};


/*
110011
110011
110011
*/
class MaskThreeHorizontalFactory: public FMF{
public:
	MaskThreeHorizontalFactory(Point ardis,ulong shift_w=1,ulong shift_h=1,double resize_factor=1.0,ulong w=3,ulong h=2);
};


/*
1111
1111
0000
0000
1111
1111
*/
class MaskThreeVerticalFactory: public FMF{
public:
	MaskThreeVerticalFactory(Point ardis,ulong shift_w=1,ulong shift_h=1,double resize_factor=1.0,ulong w=2,ulong h=3);
};


/*
0011
1100


00001111
00001111
11110000
11110000
*/
class MaskDiagonalFactory: public FMF{
public:
	MaskDiagonalFactory(Point ardis,ulong shift_w=1,ulong shift_h=1,double resize_factor=1.0,ulong w=2,ulong h=2);
	/*
	Resize the current template mask. For each horizontal stretch, it tries all the possible combinations
	of vertical strecthes.
	*/
	std::vector<Mask> resize( Mask(* fn_create)(Point));	
};



#endif