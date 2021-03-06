#ifndef __FEATURE_MASK__
#define __FEATURE_MASK__

#include <cstdio>
#include "../../util/headers/basic.h"
#include "../../util/headers/plainWriter.h"
#include "maskBlock.h"
#include "mask.h"
#include "cudaDefs.h"

/*
A FeatureMask is a rectangle composed by a list of white and black MaskBlock.
*/
class FeatureMask{
private:
	void drawOnCanvas(char* canvas,Point ardis, Point location, MaskBlock b, int color);
public:
	Mask _original_mask;
	Point _original_location;	//Mask Location (Original Ardil)
	Point _original_size;		//Mask Size (Original Ardil)

	Mask _mask;			
	Point _location;	

	ulong _id; //this id it would represent the position of this feature in the _ii_buffer of a TrainingImage

	FeatureMask(){};
	FeatureMask(Mask mask, Point location,ulong id=0);

	void drawFeatureMask(Point ardis);
	
	/*	
	When FeatureMask is created, its locations is based on the ardis used on the
	mask creation time (usually, 64x64). When detecting faces on an image, the ardis
	(now called subwindows) would be located in different coordinates, that's why we
	have to adjust it.
	*/
	void resizeMask(double factor);
	void repositionMask(Subwindow &sw);
	inline Mask mask(){return _mask;};
	inline Point location(){return _location;};
	
	static void asPlainVector(PlainWriter<ulong>& pw, const FeatureMask& fm);
	static FeatureMask fromPlainVector(PlainWriter<ulong>& pw);
};

class FeatureMaskDev{
public:
	Point _mask_size;
	Point _location;	
	ulong _white_length, _black_length;
	
	ulong _id; //this id it would represent the position of this feature in the _ii_buffer of a TrainingImage
	MaskBlockDev _white[2];
	MaskBlockDev _black[2];
	
	
	CUDA_CALLABLE_MEMBER FeatureMaskDev(){};
};

std::ostream& operator<<(std::ostream& os, const FeatureMask& fm);
std::istream& operator>>(std::istream& is, FeatureMask& fm);

#endif
