#include "../../headers/classifier/cascadeClassifier.h"

int CascadeClassifier::isFace(IntegralImage& ii, Subwindow& sw){

    std::vector<Classifier>::iterator it;
    for(it=_classifiers.begin();it!=_classifiers.end();it++){
        if( (*it).isFace(ii,sw)!=1 ){
            return 0;
        }
    }

    return 1;
}