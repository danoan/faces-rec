#ifndef __INTEGRAL_IMAGE__
#define __INTEGRAL_IMAGE__

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>

#include "../../util/headers/basic.h"
#include "../../util/headers/image.h"

#include "../../feature/headers/libfeature.h"
#include "cudaDefs.h"



class IntegralImage{
private:
    bool _copy;
    void init(ulong**& data,Point size,bool copy);
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
    IntegralImage(ulong**& data,Point size);

	inline int outsideLimits(Point& p);
	inline ulong** data(){return _data;};

	long computeBlock(std::vector<MaskBlock> &b, Point& location);
	long getFromData(Point* points);
	ulong filter(FeatureMask &fm);
};

class IntegralImageDev{
public:
    ulong* _data;
    Point _size;
    
	CUDA_CALLABLE_MEMBER_DEVICE IntegralImageDev(){};
    CUDA_CALLABLE_MEMBER_DEVICE IntegralImageDev(ulong* data,Point size):_data(data), _size(size){};

	CUDA_CALLABLE_MEMBER_DEVICE inline int outsideLimits(Point& p){return (p.y>=_size.y) || (p.y<0) || (p.x>=_size.x) || (p.x<0);};

	CUDA_CALLABLE_MEMBER_DEVICE long computeBlock(MaskBlockDev* b, ulong mask_length, Point location);
	CUDA_CALLABLE_MEMBER_DEVICE long getFromData(Point* points);
	CUDA_CALLABLE_MEMBER_DEVICE ulong filter(FeatureMaskDev* fm);
};

#endif
