#ifndef __WINDOW_CROPPER__
#define __WINDOW_CROPPER__

#include "../../util/headers/basic.h"
#include "../../util/headers/image.h"
#include <cstdlib>
#include <string>

class WindowCropper{
private:

public:
	std::string _img_path;
	Point _img_size;
	
    int _last_crop;
    int _crops_read;
    int _max_crops;
    int _random_hop;
    int _shift_step;
	Point _crop_size;
    static int prime_numbers[136]; 

    WindowCropper(std::string img_path, Point crop_size, int shift_step);
    int loadNextCrops(ulong**** data, const char* filepath, int n, int& totalRead, int& theEnd, int(* checkData)(ulong**,Point,void*), void* vp);
    int choose_random_hop(int total_crops);
};

#endif
