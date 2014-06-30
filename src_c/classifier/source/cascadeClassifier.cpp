#include "../headers/cascadeClassifier.h"

int CascadeClassifier::isFace(IntegralImage &ii, Subwindow &sw, double ac){
    
    std::vector<Classifier>::iterator it;
    for(it=_classifiers.begin();it!=_classifiers.end();it++){
        if( (*it).isFace(ii,sw,ac)!=1 ){
            return 0;
        }
    }

    return 1;    
}

int CascadeClassifier::isFace(IntegralImage &ii, double ac){
    Point wr;
    wr.x=Config::ARDIS_WIDTH;
    wr.y=Config::ARDIS_HEIGHT;

    Subwindow sw (0,0,wr,1,1);  
    return isFace(ii,sw,ac);    
}

int CascadeClassifier::isFace(IntegralImage& ii, Subwindow& sw){
    // printf("IS FACE CASCADE\n");
    std::vector<Classifier>::iterator it;
    for(it=_classifiers.begin();it!=_classifiers.end();it++){
        if( (*it).isFace(ii,sw)!=1 ){
            return 0;
        }
    }

    return 1;
}

int CascadeClassifier::isFace(IntegralImage& ii){
    Point wr;
    wr.x=Config::ARDIS_WIDTH;
    wr.y=Config::ARDIS_HEIGHT;

    Subwindow sw (0,0,wr,1,1);        
    return isFace(ii,sw);
}
    

void CascadeClassifier::save(std::string path){
    std::ofstream ofs(path.c_str());
    ofs << (*this);
    ofs.close();
}

void CascadeClassifier::load(std::string path){
    std::ifstream ifs(path.c_str());
    ifs >> (*this);
    ifs.close();    
}

std::ostream& operator<<(std::ostream& os, const CascadeClassifier& cc){
    os << cc._classifiers.size() << '\n';
    for(register int i=0;i<cc._classifiers.size();i++){
        os << cc._classifiers[i];
    }

    return os;
};

std::istream& operator>>(std::istream& is, CascadeClassifier& cc){
    int size;
    is >> size;
    for(register int i=0;i<size;i++){
        Classifier c;
        is >> c;
        cc._classifiers.push_back(c);
    }

    return is;
}