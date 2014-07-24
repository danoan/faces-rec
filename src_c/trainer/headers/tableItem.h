#ifndef __TABLE_ITEM__
#define __TABLE_ITEM__

#include "../../util/headers/basic.h"
#include "trainingImage.h"
#include "cudaDefs.h"

class TableItem{
public:
    double _weight;    
    ulong _filter_value;
    ulong _tt;
    double _error;

    int _direction;
    double _alpha;

    FeatureMask _fm;

    TableItem(){};
    TableItem(double w, ulong f, ulong tt, double e, int d):_weight(w),_filter_value(f),_tt(tt),_error(e),_direction(d){};
};

class TableItemComparator{
public:
    TableItemComparator(){};
    bool operator() (TableItem* a, TableItem* b){ return a->_filter_value < b->_filter_value; } 
};

class TableItemDev{
public:
    double _weight;    
    ulong _filter_value;
    ulong _tt;
    double _error;

    int _direction;
    double _alpha;

    ulong _id;

    CUDA_CALLABLE_MEMBER TableItemDev(){};
    CUDA_CALLABLE_MEMBER TableItemDev(double w, ulong f, ulong tt, double e, int d):_weight(w),_filter_value(f),_tt(tt),_error(e),_direction(d){};
};

#endif
