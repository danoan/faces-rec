#include "../../headers/feature/featureMask.h"

FeatureMask::FeatureMask(Mask mask,Point location,int id){
    this->_original_mask = mask;
    this->_original_location = location;
    this->_original_size = mask._size;

    this->_mask = mask;
    this->_location = location;

    this->_id = id;
}

void FeatureMask::adjustMask(Subwindow &sw){
    // Mask's Subwindow Rescale     
    ulong maskLocation[2] = { (int) floor( this->_original_location.y*sw._ce ),
                              (int) floor( this->_original_location.x*sw._ce )
                            };

    // Subwindow Location
    this->_location.y = maskLocation[0]+sw._y;
    this->_location.x = maskLocation[1]+sw._x;
                        
    
    // Mask Rescale
    this->_mask = this->_mask.rescale(sw._ce,this->_original_size);

    // printf("%.5f (%lu, %lu) (%lu, %lu)\n",sw._ce,_location.y,_location.x,_mask._size.y,_mask._size.x);
}

void FeatureMask::drawOnCanvas(char* canvas,Point ardis, Point location, MaskBlock b, int color){
    for(int i=b.a().x;i<=b.b().x;i++){
        for(int j=b.a().y;j<=b.c().y;j++){

            if(color==0){
                canvas[ (j+location.y)*ardis.x+ (i+location.x) ] = '0';
            }else{
                canvas[ (j+location.y)*ardis.x+ (i+location.x) ] = 'X';
            }
            
        }
    }
}

void FeatureMask::drawFeatureMask(Point ardis){
    int canvas_size = ardis.x*ardis.y;
    char* canvas = (char*) malloc(sizeof(char)*canvas_size);

    for(int i=0;i<canvas_size;i++){
        canvas[i]=' ';
    }
    
    printf("Size:  %d %d\n",_original_size.x,_original_size.y);
    printf("Loc:   %d %d\n",_original_location.x,_original_location.y);
    printf("Block: %d %d\n",_original_mask._white.size(),_original_mask._black.size());
    printf("White: %d %d\n",_original_mask._white[0]._w,_original_mask._white[0]._h);


    for(int i=0;i<_original_mask._white.size();i++){
        drawOnCanvas( canvas, ardis, _original_location , _original_mask._white[i], 0 );    
    }

    for(int i=0;i<_original_mask._black.size();i++){
        drawOnCanvas( canvas, ardis, _original_location , _original_mask._black[i], 1 );
    }
    
    for(int i=0;i<ardis.y;i++){
        for(int j=0;j<ardis.x;j++){
            printf("%c",canvas[i*ardis.x+j]);
        }
        printf("\n");
    }

    free(canvas);
}

std::ostream& operator<<(std::ostream& os, const FeatureMask& fm){
    os << fm._id << '\n';
    
    os << fm._original_location.x << '\n';
    os << fm._original_location.y << '\n';

    os << fm._original_size.x << '\n';
    os << fm._original_size.y << '\n';

    os << fm._location.x << '\n';
    os << fm._location.y << '\n';

    os << fm._mask;
    os << fm._original_mask;

    return os;
}

std::istream& operator>>(std::istream& is, FeatureMask& fm){
    is >> fm._id;

    is >> fm._original_location.x;
    is >> fm._original_location.y;

    is >> fm._original_size.x;
    is >> fm._original_size.y;

    is >> fm._location.x;
    is >> fm._location.y;

    is >> fm._mask;
    is >> fm._original_mask;

    return is;

}