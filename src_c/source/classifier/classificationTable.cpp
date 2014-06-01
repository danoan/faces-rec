#include "../../headers/classifier/classificationTable.h"

ClassificationTable::ClassificationTable(){
    // _tir = new TrainingImageRepository();

    _positive=0;
    _negative=0;
};

void ClassificationTable::addTrainingImage(std::string imagePath, TrainingType tit){
    // _tir->addTrainingImage(imagePath,tit);
    _images.push_back(new TrainingImage(imagePath,tit));
    if(tit==FACE) _positive+=1;
    else          _negative+=1;
}

void ClassificationTable::initTable(){
    double ipw = 1.0/(2*_positive);;
    double inw = 1.0/(2*_negative);

    Logger::debug->log("INIT TABLE: FACES: %lf SCENES: %lf\n",ipw,inw);

    _t_plus=0;
    _t_minus=0;

    // TrainingImage* t;
    std::vector<TrainingImage*>::iterator it;
    int element_order=0;
    for( it=_images.begin();it!=_images.end();it++ ){
        if( (*it)->_tt==FACE){            
            _elements.push_back( new TableItem(ipw,0.0,FACE,1.0,1,element_order++) );
            _weights.push_back(ipw);
            _t_plus+=ipw;
        }else{
            _elements.push_back( new TableItem(inw,0.0,SCENE,1.0,1,element_order++) );
            _weights.push_back(inw);
            _t_minus+=inw;
        }        
    }

    Logger::debug->log("TOTAL ELEMENTS TRAINING: %d\n",_elements.size());
}

TableItem ClassificationTable::getBestTableItem(FeatureMask& fm){
    _s_plus=0;
    _s_minus=0;

    // TrainingImage* t;
    int i=0;
    std::vector<TrainingImage*>::iterator it;
    for( it=_images.begin();it!=_images.end();it++ ){
        *(_elements[i]) = TableItem(_weights[i],  (*it)->filter(fm),  (*it)->_tt, 0, 1,i); 
        i++;
    }
    // printf("WEIGHT 1: %.12lf\n",_elements[0]->_weight);

    std::sort( _elements.begin(),_elements.end(),TableItemComparator() );
    // printf("ORDER %lu %lu %lu\n",_elements[0]->_filter_value,_elements[1]->_filter_value,_elements[2]->_filter_value);

    double e1,e2;
    TableItem bestItem(0,0,FACE,1.0, 1,-1);
    // std::string table = "";
    for(i=_elements.size()-1;i>=0;i--){
        e1 = _s_plus + (_t_minus - _s_minus);  //Error of: Everything below i is negative
        e2 = _s_minus + (_t_plus - _s_plus);    //Error of: Everything below i is positive        

        if(e1<e2){
            if(e1<bestItem._error){
                bestItem = TableItem(_elements[i]->_weight,  _elements[i]->_filter_value,  _elements[i]->_tt, e1, 1,-1);
                bestItem._fm = fm;
                // printf("E1 %.12lf\n",e1);
            }
        }else{
            if(e2<bestItem._error){
                bestItem = TableItem(_elements[i]->_weight,  _elements[i]->_filter_value,  _elements[i]->_tt, e2, -1,-1);
                bestItem._fm = fm;
                // printf("E2 %.12lf\n",e2);
            }
        }

        // char line[128];
        // std::string label = _elements[i]->_tt==FACE?"FACE":"SCENE";
        // sprintf(line, "%d \t\t %.4lf \t %lu \t\t %s \t %.4lf \t %.4lf \t %.4lf \t %.4lf \t %.4lf \t %.4lf \t %d \t \n", i,_elements[i]->_weight,_elements[i]->_filter_value,label.c_str(),_t_plus,_t_minus,_s_plus,_s_minus,e1,e2,_elements[i]->_order);
        // table = std::string(line) + table;        

        if(_elements[i]->_tt==FACE){
            _s_plus+=_elements[i]->_weight;
        }else{
            _s_minus+=_elements[i]->_weight;
        }

    }


    // table = "Order: \t Weight \t F_Value \t Label \t T_PLUS \t T_MINUS \t S_PLUS \t S_MINUS \t E1 \t E2 \t IMG_NUM \n\n" + table;
    // printf("%s\n\n",table.c_str());

    return bestItem;
}

void ClassificationTable::updateWeights(double b_t, Hypothesy& h){
    _t_plus=0;
    _t_minus=0;
    std::vector<TrainingImage*>::iterator it;
    // printf("UPDATE WEIGHTS %d %lu\n",h._direction,h._threshold);
    for(int i=0;i<_images.size();i++){
        if( h_function( _images[i]->filter(h._fm), h._direction, h._threshold )==1 ){
            // printf("IMG %d IS FACE (%lu) \n", i,(*it)->filter(h._fm));
            //Classified as Face
            if( _images[i]->_tt!=FACE ){
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
            if( _images[i]->_tt!=SCENE ){
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
    for(int i=0;i<_images.size();i++){
        _weights[ i ] *=  normal_factor;
    }

    _t_plus*= normal_factor;
    _t_minus*= normal_factor;

    Logger::debug->log("END FEATURE CHOOSEN\n NORMAL %lf T+:%lf T-%lf SUM: %lf\n",normal_factor,_t_plus,_t_minus,_t_plus+_t_minus);
}

