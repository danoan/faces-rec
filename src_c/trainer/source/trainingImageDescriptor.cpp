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

        _img_data = NULL;
    }
}

int TID::loadNextCrops(int n, int& totalRead, int& theEnd, int(* checkData)(ulong**,Point,void*), void* vp){
    ulong*** data;

    _crops.clear();
    n = n - _crops.size();

    printf("%d %d %d\n",_last_crop,_crop_size.x,_crop_size.y);

    if(_last_crop>=_max_crops){
        printf("FREE IMG DATA\n");
        theEnd=1;
        freeImgData(_img_data,_img_size);
        totalRead=0;
        return 0;
    }
    else theEnd=0;    

    totalRead = _last_crop;

    int total_crops;

    if(_img_data==NULL){
        total_crops = getImageCrops(&data,&_img_data,&_img_size,_img_path.c_str(),&_last_crop,n,_max_crops,_crop_size.x,_crop_size.y,_shift_step,checkData,vp);
    }else{
        total_crops = getImageCrops(&data,&_img_data,&_img_size,&_last_crop,n,_max_crops,_crop_size.x,_crop_size.y,_shift_step,checkData,vp,0);
    }
    for(int i=0;i<total_crops;i++){
        _crops.push_back(data[i]);;
    }

    totalRead = _last_crop - totalRead;

    return total_crops;
}