#include "../headers/classificationTable.h"

ClassificationTable::ClassificationTable(){
};

ClassificationTable::~ClassificationTable(){
		printf("DESTROY TABLE\n");
        for(register int i=0;i<_elements.size();i++){
            delete _elements[i];
        }   
        
        /*
        free(data_host);
        free(fmd_host);
        free(answer_host);
        free(answer_host_buffer);
        
        cudaFree(data_device);
        cudaFree(fmd_device);
        cudaFree(answer_device);
        cudaFree(answer_device_buffer);*/
}

void ClassificationTable::initTable(TrainingSet& ts){
    _positive = ts._faces.size();
    _negative = ts._scenes.size();

    double ipw = 1.0/(2*_positive);;
    double inw = 1.0/(2*_negative);

    Logger::debug->log("INIT TABLE: FACES: %lf SCENES: %lf\n",ipw,inw);

    _t_plus=0;
    _t_minus=0;

    for(int i=0;i<ts.size();i++){
        TrainingImage* ti = ts.get(i);

        if( ti->_tt==FACE){            
            _elements.push_back( new TableItem(ipw,0.0,FACE,1.0,1) );
            _weights.push_back(ipw);
            _t_plus+=ipw;
        }else{
            _elements.push_back( new TableItem(inw,0.0,SCENE,1.0,1) );
            _weights.push_back(inw);
            _t_minus+=inw;
        }        
    }

    Logger::debug->log("TOTAL ELEMENTS TRAINING: %d\n",_elements.size());
    //printf("BEGIN CLASSIFICATION\n");

}


TableItem ClassificationTable::getBestTableItem(FeatureMask& fm, TrainingSet& ts){
	return getBestTableItem(fm, ts, NULL, 0);
}

TableItem ClassificationTable::getBestTableItem(FeatureMask& fm, TrainingSet& ts, ulong* answer_host, int startFeature){
    _s_plus=0;
    _s_minus=0;
    // TrainingImage* t;


    //------------PARTE 1: 90% do tempo de Execucao------------
        
    TrainingImage* ti;
    for(int i=0;i<ts.size();i++){
        ti = ts.get(i);

        _elements[i]->_weight = _weights[i];
        if(answer_host!=NULL){
			
			/*
			int hj = (fm._id-startFeature)*ts.size() + i;
			int fcuda = answer_host[ (fm._id-startFeature)*ts.size() + i ];
			int fnormal = ti->filter(fm);
			
			if(fcuda!=fnormal && hj<10) printf("FID(%d-%d): %d - %d\n", fm._id,hj,fcuda,fnormal);
			*/
			 
			_elements[i]->_filter_value = answer_host[ (fm._id-startFeature)*ts.size() + i ];  //ti->filter(fm);   //Filter 70%
		}else{
			_elements[i]->_filter_value = ti->filter(fm);   //Filter 70%
		}
			
        _elements[i]->_tt = ti->_tt;
        _elements[i]->_error = 0;
        _elements[i]->_direction = 1;
    }

    // printf("WEIGHT 1: %.12lf\n",_elements[0]->_weight);
    // printf("PRE-SORT\n");
    std::sort( _elements.begin(),_elements.end(),TableItemComparator() );   //Sort 30%
    // printf("ORDER %lu %lu %lu\n",_elements[0]->_filter_value,_elements[1]->_filter_value,_elements[2]->_filter_value);

    //--------------FIM PARTE 1---------------


    //-------------PARTE 2: 10% do tempo de Execucao-------------

    double e1,e2;
    double bestError = 1.0;
    int bestErrorIndex = 0;
    int bestErrorDirection = 1;
    // std::string table = "";
    for(int i=_elements.size()-1;i>=0;i--){
        e1 = _s_plus + (_t_minus - _s_minus);  //Error of: Everything below i is negative
        e2 = _s_minus + (_t_plus - _s_plus);    //Error of: Everything below i is positive        

        if(e1<e2){
            if(e1<bestError){
                bestError = e1;
                bestErrorIndex = i;
                bestErrorDirection = 1;                
            }
        }else{
            if(e2<bestError){
                bestError = e2;
                bestErrorIndex = i;
                bestErrorDirection = -1;                
            }
        }

        if(_elements[i]->_tt==FACE){
            _s_plus+=_elements[i]->_weight;
        }else{
            _s_minus+=_elements[i]->_weight;
        }

    }

    TableItem bestItem(_elements[bestErrorIndex]->_weight,  _elements[bestErrorIndex]->_filter_value,  _elements[bestErrorIndex]->_tt, bestError, bestErrorDirection);
    bestItem._fm = fm;

    //--------------FIM PARTE 2---------------------

    // table = "Order: \t Weight \t F_Value \t Label \t T_PLUS \t T_MINUS \t S_PLUS \t S_MINUS \t E1 \t E2 \t IMG_NUM \n\n" + table;
    // printf("%s\n\n",table.c_str());

    return bestItem;
}

void ClassificationTable::updateWeights(double b_t, Hypothesy& h, TrainingSet& ts){
    _t_plus=0.0;
    _t_minus=0.0;
    std::vector<TrainingImage*>::iterator it;
    // printf("UPDATE WEIGHTS %d %lu\n",h._direction,h._threshold);
    for(int i=0;i<ts.size();i++){
        TrainingImage* ti = ts.get(i);

        if( h_function( ti->filter(h._fm), h._direction, h._threshold )==1 ){
            // printf("IMG %d IS FACE (%lu) \n", i,(*it)->filter(h._fm));
            //Classified as Face
            if( ti->_tt!=FACE ){
                //Uncorrectly Classified                
                // printf("UNCORRECTLY\n");
                _t_minus+=_weights[ i ];
            }else{
                //Correctly Classified
                // printf("CORRECTLY\n");
                _weights[ i ] *= b_t;
                _t_plus+=_weights[ i ];
            }
        }else{
            // printf("IMG %d IS SCENE (%lu) \n", i,(*it)->filter(h._fm));
            //Classified as Non-Face
            if( ti->_tt!=SCENE ){
                //Uncorrectly classified                
                // printf("UNCORRECTLY\n");
                _t_plus+=_weights[ i ];
            }else{
                //Correctly Classified
                // printf("CORRECTLY\n");
                _weights[ i ] *= b_t;
                _t_minus+=_weights[ i ];
            }
        }
    }

    //Now normalize the weights
    double normal_factor = 1.0/(_t_plus+_t_minus);
    for(int i=0;i<ts.size();i++){
        _weights[ i ] *=  normal_factor;
    }

    _t_plus*= normal_factor;
    _t_minus*= normal_factor;

    Logger::debug->log("END FEATURE CHOOSEN\n NORMAL %lf T+:%lf T-%lf SUM: %lf\n",normal_factor,_t_plus,_t_minus,_t_plus+_t_minus);
}

