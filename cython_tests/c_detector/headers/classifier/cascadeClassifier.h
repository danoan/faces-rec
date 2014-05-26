#ifndef __CASCADE_CLASSIFIER__
#define __CASCADE_CLASSIFIER__

#include "classifier.h"

#include <vector>

class CascadeClassifier{
private:
    std::vector<Classifier> _classifiers;

public:
    inline void addClassifier(Classifier& cl){_classifiers.push_back(cl);};
    int isFace(IntegralImage& ii, Subwindow& sw);
};

#endif