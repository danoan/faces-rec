#include <string>
#include <cstdio>
#include "../headers/libutil.h"

int check_data(ulong** data, Point size, void* vp){
    return 1;
}

int teste_crop_dinamico(){
    ulong*** crops;
    int d = 4;

    Point _size;
    getSize(( Config::DATASET_PATH+"/24x24/Instancia_Sun_300_400_nocrop/training_images/non_faces/9.pgm" ).c_str(),&_size);

    Point _crop_size;
    _crop_size.x = Config::ARDIS_WIDTH;
    _crop_size.y = Config::ARDIS_HEIGHT;

    int _last_crop=0;
    int _shift_step=1;

    int real_width = _size.x - _crop_size.x;
    int real_height = _size.y - _crop_size.y;

    int ie = (int) floor( (real_width*real_height)/(_shift_step*_shift_step) ) + 1;
    int _max_crops = ie + (int) floor( (real_width - (ie*_shift_step)%real_width)/_shift_step );

    printf("%d\n",_max_crops);

    int crop_start_index = 0;
    int random_hop=83;
    int total_read;
    int total_crops = getImageCrops(&crops,( Config::DATASET_PATH+"/24x24/Instancia_Sun_300_400_nocrop/training_images/non_faces/9.pgm" ).c_str(),&crop_start_index,&total_read,963,_max_crops,_crop_size.x,_crop_size.y,_shift_step,random_hop,check_data,&d);

    //Step by Step
    // struct timeval stop_t,start_t;
    // ulong seg;
    // ulong useg;

    // while(crop_start_index<_max_crops){
    //     getImageCrops(&crops,( Config::DATASET_PATH+"/sun/com_faces/1.pgm" ).c_str(),&crop_start_index,1,_max_crops,_crop_size.x,_crop_size.y,_shift_step,random_hop,check_data,&d);
    //     crop_start_index++;

    //     gettimeofday(&start_t,NULL);
    //     while(1){
    //         gettimeofday(&stop_t,NULL);
    //         seg = (stop_t.tv_sec-start_t.tv_sec);   
    //         if(seg>1) break;
    //     }
    
    // }

    printf("Total Crops: %d\n",total_crops);    
}

int teste_plain_writer(){
    PlainWriter <ulong> pw1;
    for(int i=0;i<2048;i++){
        pw1.write(i);
    }

    pw1.moveBegin();
    for(int i=0;i<2048;i++){
        printf("%d ", pw1.read());
    }

    printf("\n\n");
    pw1.moveBegin();
    for(int i=0;i<4;i++){
        printf("\nLINE %d\n", i);
        ulong* l = pw1.readLine(512);
        for(int j=0;j<512;j++){
            printf("%d ",l[j]);
        }
    }

    printf("\n\n");
    pw1.moveBegin();
    ulong** ar = pw1.readArray(128,16);
    for(int i=0;i<16;i++){
        printf("\nLINE %d\n", i);
        for(int j=0;j<128;j++){
            printf("%d ", ar[i][j]);
        }
    }

}

int main(int argc, char* argv[]){
    teste_crop_dinamico();
}
