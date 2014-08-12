#include "../headers/trainingImageDescriptor.h"

void TID::init(){	
    getSize(_img_path,&_size);
	if(_crop_selector){
        srand(time(NULL));

		Point wc_size;
		wc_size.x = _crop_size.x;
		wc_size.y = _crop_size.y;
		
		for(int i=0;i<10;i++){			
			_wc.push( WindowCropper(wc_size,1,_size) );
			wc_size.x*=1.25;
			wc_size.y*=1.25;
		}		
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
    

    if(_wc.size()==0){
        theEnd=1;
        return 0;
    }
    else theEnd = 0;
    
    int random_cropper;
    int windows_read;
    int cropper_ended;
    int r = 0;
    int crops = 0;
    while(r<n){

        if(_wc.size()==0){
            break;
        }        

        random_cropper = rand()%_wc.size();
        crops += _wc[random_cropper].loadNextCrops(&data,_img_path.c_str(), (n-r), windows_read, cropper_ended, checkData, vp);

        for(int i=0;i<crops;i++){
            _crops.push_back(data[i]);;    
        }

        r+=crops;

        if(cropper_ended==1){
            _wc.erase(_wc.begin()+random_cropper);
        }

    }

    return r;
}
