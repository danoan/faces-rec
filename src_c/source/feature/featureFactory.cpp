#include "../../headers/feature/featureFactory.h"

FMF::FMF(Mask(* fn_create)(Point), Point ardis,ulong shift_w,ulong shift_h,double resize_factor,ulong w,ulong h,ulong resize_w_step,ulong resize_h_step){
    this->_ardis = ardis;

    this->_shift_w = shift_w;
    this->_shift_h = shift_h;

    this->_resize_factor = resize_factor;
    this->_w = w;
    this->_h = h;

    this->_resize_w_step = resize_w_step;
    this->_resize_h_step = resize_h_step;

    this->_fn_create = fn_create;

    this->_rit = 0;
    this->_lit = 0;
}

std::vector<Mask> FMF::resize(Mask(* fn_create)(Point)){
    // if(this->ardis==NULL){
    //  exit(1);
    // }
    if(this->_ardis.x<2 or this->_ardis.y<2){
        exit(2);
    }

    // printf("NORMAL\n");

    ulong left_w = this->_ardis.x - this->_w + 1;
    ulong left_h = this->_ardis.y - this->_h + 1;

    ulong original_w = this->_w;
    ulong original_h = this->_h;

    std::vector<ulong> width_list = incrementList(this->_resize_factor,this->_resize_w_step,this->_w,this->_ardis.x);
    std::vector<ulong> height_list = incrementList(this->_resize_factor,this->_resize_h_step,this->_h,this->_ardis.y);


    std::vector<Mask> resize_list;
    for(int i=0;i<width_list.size();++i){
        for(int j=0;j<height_list.size();++j){
            Point size;
            size.x = width_list[i];
            size.y = height_list[j];

            resize_list.push_back( (*fn_create)(size) );
        }
    }

    return resize_list;
}

std::vector<Point> FMF::changeLocal(Point currentSize){
    // if(this->ardis==NULL){
    //  exit(1);
    // }
    if(this->_ardis.x<2 or this->_ardis.y<2){
        exit(2);
    }

    Point local;

    ulong left_w = this->_ardis.x - currentSize.x + 1;
    ulong left_h = this->_ardis.y - currentSize.y + 1;

    std::vector<Point> localPoints;

    for(int i=0;i<left_w;i+=this->_shift_w){
        for(int j=0;j<left_h;j+=this->_shift_h){
            Point local;
            local.x=i;
            local.y=j;

            localPoints.push_back(local);
        }
    }

    return localPoints;
}

std::vector<Point> FMF::changeLocal(){
    Point currentSize;
    currentSize.x = this->_w;
    currentSize.y = this->_h;

    return this->changeLocal(currentSize);
}

FeatureMask FMF::next(){
    int count=0;    

    // if(this->hasNext()==-1){
    //     return NULL;
    // }

    FeatureMask fm ( this->_resizeList[_rit], this->_localList[_lit++]);
    // printf("(%lu %lu) (%lu %lu)\n",_resizeList[_rit].size().y, _resizeList[_rit].size().x,_localList[_lit-1].y,_localList[_lit-1].x);

    if(_lit==this->_localList.size()){      
        _lit=0;
        _rit+=1;

        if(this->hasNext()==1){
            Point currentSize = this->_resizeList[_rit].size();
            this->_localList = this->changeLocal(currentSize);
        }
    }

    return fm;
}

std::vector<ulong> incrementList(double factor, ulong divisor, ulong start, ulong end){
    std::vector<double> multiplos;
    std::vector<ulong> il;
    double pivot = (double) start;

    for(int i=0;i<(end/divisor)+1;++i){
        multiplos.push_back(i*divisor);
    }

    ulong current_multiplo = (ulong) ceil( (start*1.0/divisor));
    current_multiplo = current_multiplo>0? current_multiplo: 1;

    while(current_multiplo<multiplos.size()){
        il.push_back( multiplos[current_multiplo] );
        pivot = pivot*factor;
        if( current_multiplo < (int) ceil(pivot/divisor) ){
            current_multiplo = (int) ceil(pivot/divisor);
        }else{
            current_multiplo+=1;
        }
    }

    return il;
}
