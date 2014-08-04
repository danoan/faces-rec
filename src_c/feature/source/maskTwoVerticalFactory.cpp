#include "../headers/maskTwoVerticalFactory.h"

MaskTwoVerticalFactory::MaskTwoVerticalFactory(Point ardis,ulong shift_w,ulong shift_h,double resize_factor,ulong w,ulong h):FMF(&createMaskM2V, ardis,shift_w,shift_h,resize_factor,w,h,1,2){
    if(this->_h%2!=0){
        this->_h+=1;
    }

    this->_resizeList = this->resize(_fn_create);
    this->_localList = this->changeLocal();     
}
