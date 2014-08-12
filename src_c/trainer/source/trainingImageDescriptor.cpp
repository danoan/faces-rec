#include "../headers/trainingImageDescriptor.h"

void TID::init(){	
	if(_crop_selector){
		Point wc_size;
		wc_size.x = _crop_size.x;
		wc_size.y = _crop_size.y;
		
		for(int i=0;i<10;i++){			
			_wc.push( WindowCropper(1,wc_size) );
			wc_size.x*=1.25;
			wc_size.y*=1.25;
		}
		
        _img_data = NULL;
    }	
}

TID::TID(std::string img_path, bool crop_selector, Point crop_size): _img_path(img_path), _crop_selector(crop_selector), _crop_size(crop_size){
	init();
}

TID::TID(std::string img_path, bool crop_selector): _img_path(img_path), _crop_selector(crop_selector){
	_crop_size.x = Config::ARDIS_WIDTH;
	_crop_size.y = Config::ARDIS_HEIGHT;
	
	init();
}

int TID::loadNextCrops(int n, int& totalRead, int& theEnd, int(* checkData)(ulong**,Point,void*), void* vp){
    ulong*** data;

    _crops.clear();
    n = n - _crops.size();

    printf("%d %d %d\n",_last_crop,_crop_size.x,_crop_size.y);

    if(_crops_read>=_max_crops){
        printf("FREE IMG DATA\n");
        theEnd=1;
        freeImgData(_img_data,_img_size);
        totalRead=0;
        return 0;
    }
    else theEnd=0;    

    int total_crops;
    totalRead = _crops_read;

	/*
    if(_img_data==NULL){
        total_crops = getImageCrops(&data,&_img_data,&_img_size,_img_path.c_str(),&_last_crop,&_crops_read,n,_max_crops,_crop_size.x,_crop_size.y,_shift_step,_random_hop,checkData,vp);
    }else{
        total_crops = getImageCrops(&data,&_img_data,&_img_size,&_last_crop,&_crops_read,n,_max_crops,_crop_size.x,_crop_size.y,_shift_step,_random_hop,checkData,vp,0);
    }*/
    
    total_crops = getImageCropsTest(&data,_img_path.c_str(),&_last_crop,&_crops_read,n,_max_crops,_crop_size.x,_crop_size.y,_shift_step,_random_hop,checkData,vp);

    for(int i=0;i<total_crops;i++){
        _crops.push_back(data[i]);;
    }

    totalRead = _crops_read - totalRead;

    return total_crops;
}
