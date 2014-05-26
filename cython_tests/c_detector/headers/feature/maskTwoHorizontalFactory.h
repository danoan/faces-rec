#ifndef __MASK_2HF__
#define __MASK_2HF__

#include "featureFactory.h"

/*
0011
0011
0011

It always has even width.
*/
class MaskTwoHorizontalFactory: public FMF{
public:
    MaskTwoHorizontalFactory(){};
    MaskTwoHorizontalFactory(Point ardis,ulong shift_w=1,ulong shift_h=1,double resize_factor=1.0,ulong w=2,ulong h=2);
};

#endif