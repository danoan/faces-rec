#include "../headers/trainingImageDescriptor.h"

int TID::prime_numbers[146] = {83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541,547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,641,643,647,653,659,661,673,677,683,691,701,709,719,727,733,739,743,751,757,761,769,773,787,797,809,811,821,823,827,829,839,853,857,859,863,877,881,883,887,907,911,919,929,937,941,947,953,967,971,977,983,991,997};

void TID::init(){	
	if(_crop_selector){
        getSize(_img_path.c_str(), &_size);
        srand(time(NULL));

        _shift_step=1;

        int real_width = _size.x - _crop_size.x;
        int real_height = _size.y - _crop_size.y;

        int ie = (int) floor( (real_width*real_height)/(_shift_step*_shift_step) ) + 1;
        _max_crops = ie + (int) floor( (real_width - (ie*_shift_step)%real_width)/_shift_step );

        _last_crop= (rand()%_max_crops);
        _random_hop = choose_random_hop(_max_crops);
        _crops_read = 0;

        _img_data = NULL;
    }	
}

int TID::choose_random_hop(int total_crops){
    for(int i=0;i<146;i++){
        if(total_crops%TID::prime_numbers[i]!=0){
            return TID::prime_numbers[i];
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

    if(_img_data==NULL){
        total_crops = getImageCrops(&data,&_img_data,&_img_size,_img_path.c_str(),&_last_crop,&_crops_read,n,_max_crops,_crop_size.x,_crop_size.y,_shift_step,_random_hop,checkData,vp);
    }else{
        total_crops = getImageCrops(&data,&_img_data,&_img_size,&_last_crop,&_crop_read,n,_max_crops,_crop_size.x,_crop_size.y,_shift_step,_random_hop,checkData,vp,0);
    }

    for(int i=0;i<total_crops;i++){
        _crops.push_back(data[i]);;
    }

    totalRead = _crops_read - totalRead;

    return total_crops;
}
