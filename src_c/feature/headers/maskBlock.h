#ifndef __MASK_BLOCK__
#define __MASK_BLOCK__

#include "../../util/headers/basic.h"
#include "../../util/headers/plainWriter.h"
#include "maskBlock.h"
#include "cudaDefs.h"

class MaskBlock{
public:
    Point _points[4];
    ulong _w,_h;

    MaskBlock(){};
    MaskBlock(Point a, Point b, Point c, Point d);

    inline Point a(){ return this->_points[0]; };
    inline Point b(){ return this->_points[1]; };
    inline Point c(){ return this->_points[2]; };
    inline Point d(){ return this->_points[3]; };

    inline ulong w(){ return this->_w; };
    inline void w(ulong w){this->_w=w;};

    inline ulong h(){ return this->_h; };
    inline void h(ulong h){this->_h=h;};

    static void asPlainVector(PlainWriter<ulong>& pw, const MaskBlock& m);
    static MaskBlock fromPlainVector(PlainWriter<ulong>& pw);
};

class MaskBlockDev{
public:
    Point _points[4];
    ulong _w,_h;

    CUDA_CALLABLE_MEMBER MaskBlockDev(){};
    CUDA_CALLABLE_MEMBER MaskBlockDev(Point a, Point b, Point c, Point d);
};

std::ostream& operator<<(std::ostream& os, const MaskBlock& mb);
std::istream& operator>>(std::istream& is, MaskBlock& mb);

#endif
