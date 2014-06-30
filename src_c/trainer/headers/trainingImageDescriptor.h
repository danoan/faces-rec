#ifndef __TRAINING_IMAGE_DESCRIPTOR__
#define __TRAINING_IMAGE_DESCRIPTOR__

#include <string>
#include <vector>
#include "../../util/headers/image.h"

class TID{
public:
    TID(std::string img_path, bool crop_selector);

    std::string _img_path;

    int _total_crops;
    Point _size;

    bool _crop_selector;
    int _last_crop;

    std::vector<ulong**> _crops;

    int loadNextCrops(int n);

};

#endif