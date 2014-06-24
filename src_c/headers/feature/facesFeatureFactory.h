#ifndef __FACES_FEATURE_FACTORY__
#define __FACES_FEATURE_FACTORY__

#include <vector>

#include "featureFactory.h"
#include "maskTwoHorizontalFactory.h"
#include "maskTwoVerticalFactory.h"
#include "maskThreeHorizontalFactory.h"
#include "maskThreeVerticalFactory.h"
#include "maskDiagonalFactory.h"

class FacesFeatureFactory{
private:
    int _size;
    int _nextIndex;

public:
    std::vector<FeatureMask> _facesFeatures;

    FacesFeatureFactory();

    int hasNext();
    FeatureMask next();
};

#endif