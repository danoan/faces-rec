#ifndef __MASK_DF__
#define __MASK_DF__

#include "featureFactory.h"

/*
0011
1100


00001111
00001111
11110000
11110000
*/
class MaskDiagonalFactory: public FMF{
public:
    MaskDiagonalFactory(Point ardis,ulong shift_w=1,ulong shift_h=1,double resize_factor=1.0,ulong w=2,ulong h=2);
    /*
    Resize the current template mask. For each horizontal stretch, it tries all the possible combinations
    of vertical strecthes.
    */
    std::vector<Mask> resize( Mask(* fn_create)(Point));    
};

#endif