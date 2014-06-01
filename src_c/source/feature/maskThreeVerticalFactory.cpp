#include "../../headers/feature/maskThreeVerticalFactory.h"

MaskThreeVerticalFactory::MaskThreeVerticalFactory(Point ardis,ulong shift_w,ulong shift_h,double resize_factor,ulong w,ulong h): FMF(&createMaskM3V, ardis,shift_w,shift_h,resize_factor,w,h,1,3){
    if(this->_h%3!=0){
        this->_h+= 3-this->_h%3;
    }

    this->_resizeList = this->resize(_fn_create);
    this->_localList = this->changeLocal();     
}
