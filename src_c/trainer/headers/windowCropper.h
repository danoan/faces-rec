#ifndef __WINDOW_CROPPER__
#define __WINDOW_CROPPER__

class WindowCropper{
private:

public:
    int _last_crop;
    int _crops_read;
    int _max_crops;
    int _random_hop;
    int _shift_step;
	Point _crop_size;
    static int prime_numbers[136]; 

    WindowCropper(Point crop_size, int shift_step, Point img_size);
    int loadNextCrops(ulong**** data, const char* filepath, int n, int& totalRead, int& theEnd, int(* checkData)(ulong**,Point,void*), void* vp);
};

#endif
