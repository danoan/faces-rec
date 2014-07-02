#ifndef __TRAINER_SET_CROP__
#define __TRAINER_SET_CROP__

#include <string>
#include <vector>
#include "../../classifier/headers/classifier.h"
#include "../../util/headers/directory.h"
#include "trainingSet.h"
#include "trainerSetManager.h"

class TrainerSetCrop:public TrainerSetManager{
private:

    std::string _faces_training_path;
    std::string _faces_validation_path;
    std::string _scenes_training_path;
    std::string _scenes_validation_path;

    TrainingSet _trs;
    TrainingSet _vas;

    std::vector<TID> _tid_training;
    std::vector<TID> _tid_validation;

    int _crops_per_window;
    int _ncropsScenesTraining;
    int _ncropsScenesValidation;

public:
    TrainerSetCrop(std::string faces_training_path, std::string faces_validation_path, std::string scenes_training_path, std::string scenes_validation_path, int cropsPerWindow, int ncropsScenesTraining, int ncropsScenesValidation);
    void init(int featuresNumber, int bufferSize);

    int loadAndStoreFaces(std::string dir_path,TrainingSet& ts);
    int loadScenes(std::string dir_path,std::vector<TID>& vtid);
    int generateCrops(std::vector<TID>& vtid,int imgIndex, int ncrops, int& totalGen, void* vp );
    int addSceneCrops(std::vector<TID>& vtid,int n,TrainingSet& ts, void* vp);

    int resetSets(int stage, ClassifierInterface& cc);
    bool checkClassifier(ClassifierInterface& cc, double* ac, double* fi, double* di, double max_fp_rate, double min_det_rate, double ac_start,double ac_step, double ac_min);

    TrainingSet& trs(){return _trs;};
    TrainingSet& vas(){return _vas;};
};

#endif