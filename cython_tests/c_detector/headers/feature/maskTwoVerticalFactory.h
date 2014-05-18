#ifndef __MASK_2VF__
#define __MASK_2VF__

#include "featureFactory.h"

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

#endif