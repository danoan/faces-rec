#ifndef __DRAW_FEATURE__
#define __DRAW_FEATURE__

#include <cstdio>
#include <cstdlib>
#include <string>
#include "../../feature/headers/libfeature.h"
#include "image.h"

void drawOnBox(int** boxes,int num_box,Point location, MaskBlock b, char* color);
void drawBlocks(MagickWand* image_wand, std::vector<MaskBlock> blocks, Point location, char* color);
void drawFeatureImage(FeatureMask fm, int save_or_display, std::string save_path, int ardis);

#endif