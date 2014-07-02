#include "../headers/trainingImageDescriptor.h"

TID::TID(std::string img_path, bool crop_selector): _img_path(img_path), _crop_selector(crop_selector){
    if(_crop_selector){
        getSize(_img_path.c_str(), &_size);
        _crop_size.x = Config::ARDIS_WIDTH;
        _crop_size.y = Config::ARDIS_HEIGHT;

        _last_crop=0;
        _shift_step=10;

        int real_width = _size.x - _crop_size.x;
        int real_height = _size.y - _crop_size.y;

        int ie = (int) floor( (real_width*real_height)/(_shift_step*_shift_step) ) + 1;
        _max_crops = ie + (int) floor( (real_width - (ie*_shift_step)%real_width)/_shift_step );
    }
}

int TID::refreshCrops(int(* checkData)(ulong**,Point,void*), void* vp){
    std::list<ulong**>::iterator it;

    for(it=_crops.end();it!=_crops.begin();it--){
        if( checkData(*it,_crop_size,vp)!=1 ){
            _crops.erase(it);
        }
    }    
}

int TID::loadNextCrops(int n, int& theEnd, int(* checkData)(ulong**,Point,void*), void* vp){
    ulong*** data;

    refreshCrops(checkData,vp);
    n = n - _crops.size();

    int total_crops = getImageCrops(data,_img_path.c_str(),_last_crop,n,_max_crops,_crop_size.x,_crop_size.y,_shift_step,checkData,vp);
    _last_crop+=total_crops;

    for(int i=0;i<total_crops;i++){
        _crops.push_back(data[i]);;
    }

    return total_crops;
}