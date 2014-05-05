#include <stdio.h>
#include <stdlib.h>

#include "c_basic.h"
#include "basic.h"
#include "classifier.h"

Classifier load_Classifier(const char* filename);

C_Classifier loadC_Classifier(const char* filename);
void printC_Block(C_Block* b, ulong len);
void printC_FeatureMask(C_FeatureMask fm);
void printC_Hypothesy(C_Hypothesy h);
void printC_Classifier(C_Classifier cl);
