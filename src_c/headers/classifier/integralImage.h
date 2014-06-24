#ifndef __INTEGRAL_IMAGE__
#define __INTEGRAL_IMAGE__

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>

#include "../util/basic.h"
#include "../util/image.h"

#include "../feature/libfeature.h"



class IntegralImage{
private:
	ulong** _data;
	Point _size;

public:
	IntegralImage(){};
    ~IntegralImage(){
        // printf("DESTROY!!!!!! %d\n",_size.y);
        for(int i=0;i<_size.y;i++){free(_data[i]);}
        free(_data);        
    }
	IntegralImage(std::string img_path);

	inline int outsideLimits(Point& p);
	inline ulong** data(){return _data;};

	long computeBlock(std::vector<MaskBlock> &b, Point location);
	long getFromData(Point* points);
	ulong filter(FeatureMask &fm);
};

#endif