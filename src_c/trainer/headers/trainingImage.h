#ifndef __TRAINING_IMAGE__
#define __TRAINING_IMAGE__

#include <vector>

#include "../../util/headers/plainWriter.h"
#include "../../feature/headers/libfeature.h"
#include "../../classifier/headers/integralImage.h"

#define MAX_FEATURE_VALUE 1048576 //64*64*256 Para um ardis de 64x64

#define SCENE 0
#define FACE 1

class TrainingImage{
public:
    ulong _tt;
    IntegralImage* _ii;

    std::string _img_path;
    int _features_number;

    bool _has_buffer;
    std::vector<ulong> _ii_buffer;  //if has_buffer is true, it puts the integral values here

    TrainingImage(std::string imagePath, ulong tt, bool has_buffer, int features_number);
    TrainingImage(ulong** data, Point size, ulong tt);
    ~TrainingImage(){
        // printf("DESTROY TRAINNING\n");
        delete _ii;
    }
    unsigned long int filter(FeatureMask& fm);


	static void asPlainVector(PlainWriter<ulong>& pw, const TrainingImage& ti);
	static TrainingImage fromPlainVector(PlainWriter<ulong>& pw);
};

#endif
