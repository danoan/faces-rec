#ifndef __C_LOADER__
#define __C_LOADER__

#include <stdio.h>
#include <stdlib.h>
#include "c_basic.h"

void printC_Block(C_Block* b, ulong len);
void printC_FeatureMask(C_FeatureMask fm);
void printC_Hypothesy(C_Hypothesy h);
void printC_Classifier(C_Classifier cl);

C_Classifier loadC_Classifier(const char* filename);
void saveC_Classifier(C_Classifier cl,char* filename);

#endif