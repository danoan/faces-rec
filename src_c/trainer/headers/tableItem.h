#ifndef __TABLE_ITEM__
#define __TABLE_ITEM__

#include "../../util/headers/basic.h"
#include "trainingImage.h"

class TableItem{
public:
    double _weight;    
    ulong _filter_value;
    TrainingType _tt;
    double _error;

    int _direction;
    double _alpha;

    int _order;
    FeatureMask _fm;

    TableItem(){};
    TableItem(double w, ulong f, TrainingType tt, double e, int d, int o):_weight(w),_filter_value(f),_tt(tt),_error(e),_direction(d),_order(o){};
};

class TableItemComparator{
public:
    TableItemComparator(){};
    bool operator() (TableItem* a, TableItem* b){ return a->_filter_value < b->_filter_value; } 
};

#endif