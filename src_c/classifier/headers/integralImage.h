#ifndef __INTEGRAL_IMAGE__
#define __INTEGRAL_IMAGE__

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>

#include "../../util/headers/basic.h"
#include "../../util/headers/image.h"

#include "../../feature/headers/libfeature.h"



class IntegralImage{
private:
    bool _copy;
public:
    ulong** _data;
    Point _size;
    
	IntegralImage(){};
    ~IntegralImage(){
        // printf("DESTROY!!!!!! %d\n",_size.y);
        if(_copy){
            for(int i=0;i<_size.y;i++){free(_data[i]);}
            free(_data);                    
        }
    }
	IntegralImage(std::string img_path);
    IntegralImage(ulong**& data,Point size,bool copy);
    IntegralImage(ulong**& data,Point size):IntegralImage(data,size,true){};

	inline int outsideLimits(Point& p);
	inline ulong** data(){return _data;};

	long computeBlock(std::vector<MaskBlock> &b, Point location);
	long getFromData(Point* points);
	ulong filter(FeatureMask &fm);
};

#endif