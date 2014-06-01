#ifndef __LOADER__
#define __LOADER__

#include <stdio.h>
#include <stdlib.h>

#include "../basic.h"
#include "../pyToC/c_basic.h"
#include "../pyToC/c_loader.h"

#include "classifier.h"

Classifier load_Classifier(const char* filename);

Point cppPoint(C_Point cp);
void cppAddBlocks(C_Block* c_blocks, ulong c_len, Mask* m, int blockType);

#endif