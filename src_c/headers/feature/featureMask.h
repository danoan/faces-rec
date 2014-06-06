#ifndef __FEATURE_MASK__
#define __FEATURE_MASK__

#include <cstdio>
#include "../basic.h"
#include "maskBlock.h"
#include "mask.h"

/*
A FeatureMask is a rectangle composed by a list of white and black MaskBlock.
*/
class FeatureMask{
public:
	Mask _original_mask;
	Point _original_location;
	Point _original_size;

	Mask _mask;
	Point _location;

	int _id; //this id it would represent the position of this feature in the _ii_buffer of a TrainingImage

	FeatureMask(){};
	FeatureMask(Mask mask,Point location,int id=0);

	/*	
	When FeatureMask is created, its locations is based on the ardis used on the
	mask creation time (usually, 64x64). When detecting faces on an image, the ardis
	(now called subwindows) would be located in different coordinates, that's why we
	have to adjust it.
	*/
	void adjustMask(Subwindow &sw);
	inline Mask mask(){return _mask;};
	inline Point location(){return _location;};
};

#endif