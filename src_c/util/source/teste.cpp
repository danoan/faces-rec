#include <string>
#include <cstdio>
#include "../headers/libutil.h"

int check_data(ulong** data){
    return 1;
}

int main(int argc, char* argv[]){
    ulong*** crops;
    int total_crops = getImageCrops(crops,( Config::DATASET_PATH+"/sun/com_faces/0.pgm" ).c_str(),0,100,100,64,64,64,check_data);

    printf("Total Crops: %d\n",total_crops);
}