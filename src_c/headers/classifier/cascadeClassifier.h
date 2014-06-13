#ifndef __CASCADE_CLASSIFIER__
#define __CASCADE_CLASSIFIER__

#include "classifier.h"

#include <string>
#include <fstream>
#include <vector>

class CascadeClassifier:public ClassifierInterface{
public:
    std::vector<Classifier> _classifiers;

    inline void addClassifier(Classifier& cl){_classifiers.push_back(cl);};
    int isFace(IntegralImage& ii, Subwindow& sw);
    int isFace(IntegralImage& ii);

    int isFace(IntegralImage &ii, Subwindow &sw, double ac);
    int isFace(IntegralImage &ii, double ac);    

    void save(std::string path);
    void load(std::string path);
};

std::ostream& operator<<(std::ostream& os, const CascadeClassifier& cc);
std::istream& operator>>(std::istream& is, CascadeClassifier& cc);
#endif