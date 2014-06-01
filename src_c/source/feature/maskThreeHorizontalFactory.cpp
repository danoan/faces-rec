#include "../../headers/feature/maskThreeHorizontalFactory.h"

MaskThreeHorizontalFactory::MaskThreeHorizontalFactory(Point ardis,ulong shift_w,ulong shift_h,double resize_factor,ulong w,ulong h):FMF(&createMaskM3H, ardis,shift_w,shift_h,resize_factor,w,h,3,1){
    if(this->_w%3!=0){
        this->_w+= 3-this->_w%3;
    }

    this->_resizeList = this->resize(_fn_create);
    this->_localList = this->changeLocal();     
}