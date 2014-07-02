#ifndef __TRAINER_SET_STATIC__
#define __TRAINER_SET_STATIC__

#include <string>
#include <vector>
#include "trainerSetManager.h"
#include "trainingSet.h"
#include "../../classifier/headers/libclassifier.h"
#include "../../util/headers/directory.h"

class TrainerSetStatic:public TrainerSetManager{
private:
    int loadAndStorageFaces(std::string dir_path, TrainingSet& ts);
    int loadAndStorageScenes(std::string dir_path, TrainingSet& ts);
    int loadAndStorageScenes(std::string dir_path, TrainingSet& ts, ClassifierInterface& cc);

    void storeFalsePositives(ClassifierInterface& cc, std::vector<TrainingImage*>& all_examples, std::vector<TrainingImage*>& fp_examples);

    std::string currentValidationGroupPath();
    std::string currentTrainingGroupPath();
    void scanGroups(std::string groupsPath, std::vector<std::string>& container);

    std::vector<std::string> _training_group_paths;
    std::vector<std::string> _validation_group_paths;

    std::string _faces_training_path;
    std::string _faces_validation_path;
    std::string _scenes_training_path;
    std::string _scenes_validation_path;    

    int _training_group_number;
    int _validation_group_number;
    int _theEnd;

    TrainingSet _trs;
    TrainingSet _vas;    
public:
    TrainerSetStatic(){};
    TrainerSetStatic(std::string, std::string, std::string, std::string);
    void init(int featuresNumber, int bufferSize);

    int resetSets(int stage, ClassifierInterface& cc);
    bool checkClassifier(ClassifierInterface& cc, double* ac, double* fi, double* di, double max_fp_rate, double min_det_rate, double ac_start,double ac_step, double ac_min);

    TrainingSet& trs(){return _trs;};
    TrainingSet& vas(){return _vas;};
};

#endif