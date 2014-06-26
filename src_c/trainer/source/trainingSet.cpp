#include "../headers/trainingSet.h"

TrainingSet::TrainingSet(std::string facesTrainingDir, std::string scenesTrainingDir): ImageSet(facesTrainingDir, scenesTrainingDir){
    getAllPathFaces(facesTrainingDir, _faces);    
    getGroupScenePath(scenesTrainingDir, 0, -1, _scenes);
}

void TrainingSet::storeFalsePositives(ClassifierInterface& cc, std::vector<std::string>& all_examples, std::vector<std::string>& fp_examples){
    for(register int i=all_examples.size()-1;i>=0;i--){
        IntegralImage ii( all_examples[i] );
        if( cc.isFace(ii)==1 ){
            fp_examples.push_back( all_examples[i] );
        }
    }            
}

int TrainingSet::resetScenesSet(ClassifierInterface& cc, ValidationSet& vs, int stage, int training_set_size){
    std::vector<std::string> all_fp;

    storeFalsePositives(cc,vs._scenes,all_fp); //Pega FP do conjunto de validacao
    storeFalsePositives(cc,_scenes,all_fp);    //Pega FP do conjunto de treinamento

    Logger::debug->log("STAGE %d\n (FP: %d/%d)\n",stage,all_fp.size(),vs._scenes.size() + _scenes.size());
    printf("%d\n",training_set_size);
    _scenes.clear();
    int before_size = all_fp.size();    

    std::vector<std::string> new_group;
    getGroupScenePath(_svd, stage, training_set_size, new_group);   

    // printf("%d %d\n",new_group.size(),all_fp.size());

    //Irei adicionar apenas os falsos positivos do new_group no grupo de treinamento
    storeFalsePositives(cc,new_group,all_fp);

    _scenes = all_fp;

    if(_scenes.size()==before_size){
        printf("group_%d not found\n",stage);
        return -1;
    }else{
        printf("Tamanho Conj. Treinamento(Stage %d): %d\n",stage,_scenes.size());
    }

    return 1;
}