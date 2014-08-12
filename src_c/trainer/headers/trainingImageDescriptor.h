#ifndef __TRAINING_IMAGE_DESCRIPTOR__
#define __TRAINING_IMAGE_DESCRIPTOR__

#include <string>
#include <cstdlib>
#include <list>
#include <vector>
#include "../../util/headers/image.h"
#include "windowCropper.h"

class TID{
public:
    TID(std::string img_path, bool crop_selector);
    TID(std::string img_path, bool crop_selector, Point crop_size);
    void init();

    Point _img_size;

    std::string _img_path;
    Point _size;

    bool _crop_selector;
    Point _crop_size;

	std::vector<WindowCropper> _wc;
    std::list<ulong**> _crops;

    int loadNextCrops(int n, int& totalRead, int& theEnd, int(* checkData)(ulong**,Point,void*), void* vp);
};

#endif
