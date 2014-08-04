#include "../headers/maskDiagonalFactory.h"

MaskDiagonalFactory::MaskDiagonalFactory(Point ardis,ulong shift_w,ulong shift_h,double resize_factor,ulong w,ulong h): FMF(&createMaskMD, ardis,shift_w,shift_h,resize_factor,w,h,2,2){
    if(this->_w%2!=0){
        this->_w+=1;
    }

    if(this->_h%2!=0){
        this->_h+=1;
    }

    this->_resizeList = this->resize(_fn_create);
    this->_localList = this->changeLocal();     
}

std::vector<Mask> MaskDiagonalFactory::resize(Mask(* fn_create)(Point)){
    // if(this->_ardis==NULL){
    //  exit(1);
    // }
    if(this->_ardis.x<2 || this->_ardis.y<2){
        exit(2);
    }

    // printf("DIAGONAL\n");

    ulong left_w = this->_ardis.x - this->_w + 1;
    ulong left_h = this->_ardis.y - this->_h + 1;

    ulong original_w = this->_w;
    ulong original_h = this->_h;
        
    //std::vector<ulong> width_list = incrementList(this->_resize_factor,this->_resize_w_step,this->_w,this->_ardis.x);   

    std::vector<Mask> resize_list;
    for(int i=_w;i<=_ardis.x;i+=_resize_w_step){
		for(int j=_h;j<=_ardis.y;j+=_resize_h_step){
			Point size;
			size.x = i;
			size.y = j;

			resize_list.push_back( (*fn_create)(size) );
		}
    }

    return resize_list;
}
