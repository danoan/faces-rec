#ifndef __TRAINING_IMAGE_DESCRIPTOR__
#define __TRAINING_IMAGE_DESCRIPTOR__

#include <string>
#include <list>
#include "../../util/headers/image.h"

class TID{
public:
    TID(std::string img_path, bool crop_selector);

    std::string _img_path;
    Point _size;

    bool _crop_selector;
    int _last_crop;
    int _max_crops;
    int _shift_step;
    Point _crop_size;


    std::list<ulong**> _crops;

    int loadNextCrops(int n, int& theEnd, int(* checkData)(ulong**,Point,void*), void* vp);
    int refreshCrops(int(* checkData)(ulong**,Point,void*), void* vp);

};

#endif