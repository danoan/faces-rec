#include "../../headers/feature/featureMask.h"

FeatureMask::FeatureMask(Mask mask,Point location){
    this->_original_mask = mask;
    this->_original_location = location;
    this->_original_size = mask._size;

    this->_mask = mask;
    this->_location = location;
}

void FeatureMask::adjustMask(Subwindow &sw){
    // Mask's Subwindow Rescale     
    ulong maskLocation[2] = { (int) floor( this->_original_location.y*sw._ce ),
                              (int) floor( this->_original_location.x*sw._ce )
                            };

    // Subwindow Location
    this->_location.y = maskLocation[0]+sw._y;
    this->_location.x = maskLocation[1]+sw._x;
                        
    
    // Mask Rescale
    this->_mask = this->_mask.rescale(sw._ce,this->_original_size);

    // printf("%.5f (%lu, %lu) (%lu, %lu)\n",sw._ce,_location.y,_location.x,_mask._size.y,_mask._size.x);
}