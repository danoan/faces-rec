#ifndef __INTEGRAL_IMAGE__
#define __INTEGRAL_IMAGE__

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>

#include "../basic.h"
#include "../image.h"

#include "../feature/libfeature.h"



class IntegralImage{
private:
	ulong** _data;
	Point _size;

public:
	IntegralImage(){};
	IntegralImage(std::string img_path);

	inline int outsideLimits(Point p);
	inline ulong** data(){return _data;};

	long computeBlock(std::vector<MaskBlock> &b, Point location);
	long getFromData(Point points[4]);
	ulong filter(FeatureMask &fm);
};

#endif