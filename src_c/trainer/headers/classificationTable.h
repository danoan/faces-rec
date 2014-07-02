#ifndef __CLASSIFICATION_TABLE__
#define __CLASSIFICATION_TABLE__

#include <cstdlib>
#include <vector>
#include <algorithm>

#include "../../classifier/headers/classifier.h"
#include "trainingImage.h"
#include "trainingSet.h"
#include "tableItem.h"

class ClassificationTable{
private:
    std::vector<TableItem*> _elements;
    std::vector<double> _weights;
    double _t_plus,_t_minus;

    int _positive,_negative;
    double _s_plus,_s_minus;    

public:    
    ClassificationTable();
    ~ClassificationTable(){
        for(register int i=0;i<_elements.size();i++){
            delete _elements[i];
        }   
    }
    void addTrainingImage(std::string imagePath, TrainingType tit);

    void initTable(TrainingSet& ts);
    TableItem getBestTableItem(FeatureMask& fm, TrainingSet& ts);
    void updateWeights(double b_t, Hypothesy& best, TrainingSet& ts);

};

#endif