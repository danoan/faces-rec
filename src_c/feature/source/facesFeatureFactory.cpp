#include "../headers/facesFeatureFactory.h"

FacesFeatureFactory::FacesFeatureFactory(){
    Point _ardis;
    _ardis.x = Config::ARDIS_WIDTH; _ardis.y = Config::ARDIS_HEIGHT;

    int _shift_w = Config::CLASSIFIER_SHIFT_STEP;
    int _shift_h = Config::CLASSIFIER_SHIFT_STEP;

    double _resize_factor = Config::CLASSIFIER_RESIZE_FACTOR;
    int _start_w = Config::CLASSIFIER_SUBWINDOW_START_WIDTH;
    int _start_h = Config::CLASSIFIER_SUBWINDOW_START_HEIGHT;    

    double _final_fp_rate = Config::CLASSIFIER_FINAL_FALSE_POSITIVE_RATE;
    double _final_det_rate = Config::CLASSIFIER_FINAL_DETECTION_RATE;

    double _max_fp_rate = Config::CLASSIFIER_STAGE_MAX_FALSE_POSITIVE_RATE;
    double _min_det_rate = Config::CLASSIFIER_STAGE_MIN_DETECTION_RATE;       

    FMF factories[5];

    factories[0] = MaskTwoHorizontalFactory(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);    
    factories[1] = MaskTwoVerticalFactory(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);    
    factories[2] = MaskThreeHorizontalFactory(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);    
    factories[3] = MaskThreeVerticalFactory(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);    
    factories[4] = MaskDiagonalFactory(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h); 


    int counter=0;
    for(int i=0;i<5;i++){
        while( factories[i].hasNext()==1 ){
            _facesFeatures.push_back( factories[i].next(counter++) );
        }        
    }

    _nextIndex=0;
    _size=_facesFeatures.size();
    printf("FACES FACTORY %d %d\n",_size, _facesFeatures[_facesFeatures.size()-1]._id);

}

FeatureMask FacesFeatureFactory::next(){
    if(_nextIndex>=_facesFeatures.size()) printf("MAIOAIOIOA \n");
    return _facesFeatures[_nextIndex++];
}

int FacesFeatureFactory::hasNext(){
    int r = _nextIndex>=_size?-1:1;    
    if(r==-1){
        _nextIndex=0;   //Automatic Restart
    }
    return r;
}
