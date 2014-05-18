#ifndef __MASK_3HF__
#define __MASK_3HF__

#include "featureFactory.h"

/*
101
101
*/
class MaskThreeHorizontalFactory: public FMF{
public:
    MaskThreeHorizontalFactory(Point ardis,ulong shift_w=1,ulong shift_h=1,double resize_factor=1.0,ulong w=3,ulong h=2);
};

#endif