#include "../headers/classifier.h"


int Classifier::isFace(IntegralImage &ii){
	Point wr;
    wr.x=Config::ARDIS_WIDTH;
    wr.y=Config::ARDIS_HEIGHT;

    Subwindow sw (0,0,wr,1,1);        
    return isFace(ii,sw,_ac);
}

int Classifier::isFace(IntegralImage &ii, double ac){
	Point wr;
    wr.x=Config::ARDIS_WIDTH;
    wr.y=Config::ARDIS_HEIGHT;

    Subwindow sw (0,0,wr,1,1);        
    return isFace(ii,sw,ac);	
}

int Classifier::isFace(IntegralImage &ii, Subwindow &sw) {
    return isFace(ii,sw,_ac);
}
    
int Classifier::isFace(IntegralImage &ii, Subwindow &sw, double ac){
	ulong t;
	long d;
	double a;
	FeatureMask fm;

	double sx=0;
	double sa=0;

	Hypothesy h;
	for(register int i=0;i<_hypothesis.size();++i){
		h = _hypothesis[i];

		t=h._threshold;
		d=h._direction;
		a=h._alpha;
		fm=h._fm;

		fm.adjustMask(sw);
		sx+= a*h_function(ii.filter(fm),d,t);		

		// printf("%d %lf %lu %ld %lu\n",h_function(ii.filter(fm),d,t), a,ii.filter(fm),d,t);
		sa+= a;
	}

	// printf("%.11f %lu %lu (%lu %lu)\n", sx, sw._y,sw._x,sw._size.y,sw._size.x);
	if(sx>=(ac*sa)){
		return 1;
	}else{
		return 0;
	}
}

std::ostream& operator<<(std::ostream& os, const Hypothesy& h){
    os << h._threshold << '\n';
    os << h._direction << '\n';
    os << h._alpha << '\n';
    os << h._fm << '\n';

    return os;
}

std::istream& operator>>(std::istream& is, Hypothesy& h){
    is >> h._threshold;
    is >> h._direction;
    is >> h._alpha;
    is >> h._fm;

    return is;
}

std::ostream& operator<<(std::ostream& os, const Classifier& c){
    os << c._ac << '\n';
    
    os << c._ardis.x << '\n';
    os << c._ardis.y << '\n';

    os << c._hypothesis.size() << '\n';
    for(register int i=0;i<c._hypothesis.size();i++){
        os << c._hypothesis[i];
    }

    return os;
}

std::istream& operator>>(std::istream& is, Classifier& c){    
    is >> c._ac;

    is >> c._ardis.x;
    is >> c._ardis.y;

    int size;
    is >> size;
    for(register int i=0;i<size;i++){
        Hypothesy h;
        is >> h;
        c._hypothesis.push_back(h);
    }

    return is;
}
