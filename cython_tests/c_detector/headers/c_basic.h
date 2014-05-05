#ifndef __C_BASIC
#define __C_BASIC

typedef unsigned long ulong;

typedef struct{
	ulong x;
	ulong y;
} C_Point;

typedef struct{
	C_Point points[4];
	ulong w,h;
} C_Block;

typedef struct{
	C_Block* white;
	C_Block* black;
	C_Point size;

	int maskType;

	ulong len_white;
	ulong len_black;
} C_Mask;

typedef struct{
	C_Mask __original_mask;
	C_Point __original_location;
	C_Point __original_size;

	C_Mask mask;
	C_Point location;

} C_FeatureMask;

typedef struct{
	ulong threshold;
	long direction;
	double alpha;
	C_FeatureMask fm;

} C_Hypothesy;

typedef struct{
	ulong final;
	C_Point ardis;
	C_Hypothesy* hypothesis;

	ulong len_hypothesis;
} C_Classifier;

#endif