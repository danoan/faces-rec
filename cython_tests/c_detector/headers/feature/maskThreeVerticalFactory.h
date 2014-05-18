#ifndef __MASK_3VF__
#define __MASK_3VF__

#include "featureFactory.h"

/*
1111
1111
0000
0000
1111
1111
*/
class MaskThreeVerticalFactory: public FMF{
public:
    MaskThreeVerticalFactory(Point ardis,ulong shift_w=1,ulong shift_h=1,double resize_factor=1.0,ulong w=2,ulong h=3);
};

#endif