#include "../../headers/feature/maskTwoHorizontalFactory.h"

MaskTwoHorizontalFactory::MaskTwoHorizontalFactory(Point ardis,ulong shift_w,ulong shift_h,double resize_factor,ulong w,ulong h):FMF(&createMaskM2H, ardis,shift_w,shift_h,resize_factor,w,h,2,1){
    if(this->_w%2!=0){
        this->_w+=1;
    }

    this->_resizeList = this->resize( _fn_create);  
    this->_localList = this->changeLocal(); 

    printf("LEN %lu, %lu\n",_resizeList.size(),_localList.size());
}