#include "../headers/windowCropper.h"

int WindowCropper::prime_numbers[136] = {137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541,547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,641,643,647,653,659,661,673,677,683,691,701,709,719,727,733,739,743,751,757,761,769,773,787,797,809,811,821,823,827,829,839,853,857,859,863,877,881,883,887,907,911,919,929,937,941,947,953,967,971,977,983,991,997};

WindowCropper::WindowCropper(Point crop_size, int shift_step, Point img_size):_crop_size(crop_size),_shift_step(shift_step){
	srand(time(NULL));

	int real_width = img_size.x - _crop_size.x;
	int real_height = img_size.y - _crop_size.y;

	int ie = (int) floor( (real_width*real_height)/(_shift_step*_shift_step) ) + 1;
	_max_crops = ie + (int) floor( (real_width - (ie*_shift_step)%real_width)/_shift_step );

	_last_crop= (rand()%_max_crops);
	_random_hop = choose_random_hop(_max_crops);
	_crops_read = 0;
}

int WindowCropper::choose_random_hop(int total_crops){
    for(int i=0;i<136;i++){
        if(total_crops%WindowCropper::prime_numbers[i]!=0){
            return WindowCropper::prime_numbers[i];
        }
    }
}

int WindowCropper::loadNextCrops(ulong**** data, const char* filepath, int n, int& totalRead, int& theEnd, int(* checkData)(ulong**,Point,void*), void* vp){    
    if(_crops_read>=_max_crops){
        printf("FREE IMG DATA\n");
        theEnd=1;
        totalRead=0;
        return 0;
    }
    else theEnd=0;    

    int total_crops;
    totalRead = _crops_read;
    
    total_crops = getImageCropsTest(data,_img_path.c_str(),&_last_crop,&_crops_read,n,_max_crops,_crop_size.x,_crop_size.y,_shift_step,_random_hop,checkData,vp);

    totalRead = _crops_read - totalRead;

    return total_crops;
}
