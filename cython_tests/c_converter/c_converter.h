
typedef unsigned long ulong;

ulong c_len_white_blocks;
ulong c_len_black_blocks;

typedef struct{
	ulong x;
	ulong y;
} Point;

typedef struct{
	Point points[4];
	ulong w,h;
} Block;

typedef struct{
	Block* white;
	Block* black;
	Point size;

	int maskType;

	ulong len_white;
	ulong len_black;
} Mask;

typedef struct{
	Mask __original_mask;
	Point __original_location;
	Point __original_size;

	Mask mask;
	Point location;

} FeatureMask;

typedef struct{
	ulong threshold;
	long direction;
	double alpha;
	FeatureMask fm;
} Hypothesy;

typedef struct{
	ulong final;
	Point ardis;
	Hypothesy* hypothesis;

	ulong len_hypothesis;
} Classifier;