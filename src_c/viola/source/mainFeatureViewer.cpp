#include "../../util/headers/libutil.h"

int main(int argc, char* argv[]){
    if(Config::readInput(argc,argv)!=1) return 1;
    
    FacesFeatureFactory fff;
    FeatureMask fm;
    fm = fff.next();

    drawFeatureImage(fff._facesFeatures[Config::VIEWER_FEATURE_ID],1,Config::PROJECT_PATH + "/analysis/feature_images/", Config::ARDIS_WIDTH);    
}