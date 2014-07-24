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

    std::vector<TID> _buffer_training;
    std::vector<TID> _buffer_validation;

    int _nFacesTraining;
    int _nFacesValidation;
    int _ncropsScenesTraining;
    int _ncropsScenesValidation;
    int _firstClassifierScenesCrops;

    int _theEnd;
    int _totalRead;

    void storeFalsePositives(ClassifierInterface& cc, std::vector<TrainingImage*>& all_examples, std::vector<TrainingImage*>& fp_examples);
    int chargeBuffer(std::vector<TID>& vtid, std::vector<TID>& buffer, int n);

public:
    TrainerSetCrop(std::string faces_training_path, std::string faces_validation_path, std::string scenes_training_path, std::string scenes_validation_path, int nFacesTraining, int nFacesValidation, int ncropsScenesTraining, int ncropsScenesValidation, int firstClassifierScenesCrops);
    void init(int featuresNumber, int bufferSize);

    int loadAndStoreFaces(std::string dir_path,int n,TrainingSet& ts);
    int loadScenes(std::string dir_path,std::vector<TID>& vtid);
    int generateCrops(std::vector<TID>& vtid,int imgIndex, int ncrops, int& totalGen, void* vp );
    int addSceneCrops(std::vector<TID>& vtid,std::vector<TID>& buffer,int n,TrainingSet& ts, void* vp);

    int resetSets(int stage, ClassifierInterface& cc);
    bool checkClassifier(ClassifierInterface& cc, double* ac, double* fi, double* di, double max_fp_rate, double min_det_rate, double ac_start,double ac_step, double ac_min);

    TrainingSet& trs(){return _trs;};
    TrainingSet& vas(){return _vas;};
};

#endif
