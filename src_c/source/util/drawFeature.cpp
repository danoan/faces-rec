#include "../../headers/util/drawFeature.h"

void drawOnBox(int** boxes,int num_box,Point location, MaskBlock b, char* color){
    boxes[num_box][0] = b.a().x + location.x;
    boxes[num_box][1] = b.a().y + location.y;
    boxes[num_box][2] = boxes[num_box][0] + b._w;
    boxes[num_box][3] = boxes[num_box][1] +b._h;
}

void drawBlocks(MagickWand* image_wand, std::vector<MaskBlock> blocks, Point location, char* color){
    int num_boxes = blocks.size();
    
    int** boxes = (int**) malloc(num_boxes*sizeof(int*));
    for(int i=0;i<num_boxes;i++){
        boxes[i] = (int*) malloc(sizeof(int)*4);
    }
 
    for(int i=0;i<blocks.size();i++){
        drawOnBox(boxes,i,location,blocks[i],color);
    }

    drawRectangles(image_wand,num_boxes,boxes,color);

    for(int i=0;i<num_boxes;i++){
        free(boxes[i]);
    }
    free(boxes);    
}

void drawFeatureImage(FeatureMask fm, int saveOrDisplay, std::string save_path, int ardis){
    MagickWand* image_wand;
    image_wand = NewMagickWand();

    if(ardis==64){
        MagickReadImage(image_wand,"/home/daniel/Projects/faces-rec/dataset/64x64/lfw_faces/Abdel_Madi_Shabneh_0001.pgm");     
    }else{
        MagickReadImage(image_wand,"/home/daniel/Projects/faces-rec/dataset/24x24/lfw_faces/Abdel_Madi_Shabneh_0001.pgm");     
    }
    
    
    drawBlocks(image_wand,fm._original_mask._white,fm._original_location,"white");
    drawBlocks(image_wand,fm._original_mask._black,fm._original_location,"black");


    char final_path[128];
    sprintf(final_path,"%s/%d.pgm",save_path.c_str(),fm._id);

    FILE* f;
    f = fopen(final_path,"wb");

    if(saveOrDisplay==0){
        MagickWriteImageFile(image_wand,f);
    }else{
        MagickDisplayImage(image_wand,"");    
    }

    fclose(f);
    DestroyMagickWand(image_wand);     
}

// int main(){
//     FacesFeatureFactory fff;
//     FeatureMask fm;
//     fm = fff.next();

//     drawFeatureImage(fff._facesFeatures[104242]);
//     return 0;
// }
