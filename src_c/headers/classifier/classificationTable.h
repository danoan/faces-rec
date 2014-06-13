#ifndef __CLASSIFICATION_TABLE__
#define __CLASSIFICATION_TABLE__

#include <cstdlib>
#include <vector>
#include <algorithm>

#include "classifier.h"
#include "trainingImage.h"
#include "trainingImageRepository.h"
#include "tableItem.h"

class ClassificationTable{
private:
    std::vector<TableItem*> _elements;
    std::vector<double> _weights;
    double _t_plus,_t_minus;

    // TrainingImageRepository* _tir;
    std::vector<TrainingImage*> _images;
    int _positive,_negative;
    double _s_plus,_s_minus;

public:    
    ClassificationTable();
    ~ClassificationTable(){
        for(register int i=0;i<_elements.size();i++){
            delete _elements[i];
        }

        for(register int i=0;i<_images.size();i++){
            delete _images[i];
        }        
    }
    void addTrainingImage(std::string imagePath, TrainingType tit);

    void initTable();
    TableItem getBestTableItem(FeatureMask& fm);
    void updateWeights(double b_t, Hypothesy& best);

};

#endif