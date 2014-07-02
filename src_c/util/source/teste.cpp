#include <string>
#include <cstdio>
#include "../headers/libutil.h"

int check_data(ulong** data, Point size, void* vp){
    return 1;
}

int main(int argc, char* argv[]){
    ulong*** crops;
    int d = 4;

    Point _size;
    getSize(( Config::DATASET_PATH+"/sun/com_faces/0.pgm" ).c_str(),&_size);

    Point _crop_size;
    _crop_size.x = Config::ARDIS_WIDTH;
    _crop_size.y = Config::ARDIS_HEIGHT;

    int _last_crop=0;
    int _shift_step=10;

    int real_width = _size.x - _crop_size.x;
    int real_height = _size.y - _crop_size.y;

    int ie = (int) floor( (real_width*real_height)/(_shift_step*_shift_step) ) + 1;
    int _max_crops = ie + (int) floor( (real_width - (ie*_shift_step)%real_width)/_shift_step );

    printf("%d\n",_max_crops);

    int total_crops = getImageCrops(crops,( Config::DATASET_PATH+"/sun/com_faces/0.pgm" ).c_str(),35000,7428,37428,_crop_size.x,_crop_size.y,_shift_step,check_data,&d);

    printf("Total Crops: %d\n",total_crops);
}