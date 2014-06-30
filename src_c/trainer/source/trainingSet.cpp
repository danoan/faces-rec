#include "../headers/trainingSet.h"

TrainingSet::TrainingSet(std::string facesTrainingDir, std::string scenesTrainingDir): ImageSet(facesTrainingDir, scenesTrainingDir){
    _theEnd = 0;
    getAllPathFaces(facesTrainingDir, _faces);    
    getGroupScenePath(scenesTrainingDir, 0, -1, _scenes);
}

void TrainingSet::storeFalsePositives(ClassifierInterface& cc, std::vector<TID>& all_examples, std::vector<TID>& fp_examples){
    for(register int i=all_examples.size()-1;i>=0;i--){
        IntegralImage ii( all_examples[i]._img_path );
        if( cc.isFace(ii)==1 ){
            fp_examples.push_back( all_examples[i] );
        }
    }            
}

int TrainingSet::resetScenesSet(ClassifierInterface& cc, ValidationSet& vs){
    if(_theEnd==1) return -1;
    std::vector<TID> all_fp;

    storeFalsePositives(cc,vs._scenes,all_fp); //Pega FP do conjunto de validacao
    storeFalsePositives(cc,_scenes,all_fp);    //Pega FP do conjunto de treinamento

    Logger::debug->log("USING UNTIL GROUP: %d\n (FP: %d/%d)\n",_group,all_fp.size(),vs._scenes.size() + _scenes.size());
    _scenes.clear();
    int r;

    while(all_fp.size()<Config::CLASSIFIER_SCENE_MIN_TRAINING_SET_ELEMENTS){
        std::vector<TID> new_group;
        r = getGroupScenePath(_svd, _group++, -1, new_group);   

        if(r==-1){
            _theEnd=1;
            printf("REACH END OF THE GROUP\n");
            break;            
        }

        //Irei adicionar apenas os falsos positivos do new_group no grupo de treinamento
        storeFalsePositives(cc,new_group,all_fp);
    }

    _scenes = all_fp;
    printf("Tamanho Conj. Treinamento(Last group being Used %d): %d\n",_group,_scenes.size());

    return 1;
}