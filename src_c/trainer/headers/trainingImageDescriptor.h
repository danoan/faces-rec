#ifndef __TRAINING_IMAGE_DESCRIPTOR__
#define __TRAINING_IMAGE_DESCRIPTOR__

#include <string>
#include <list>
#include "../../util/headers/image.h"

class TID{
public:
    TID(std::string img_path, bool crop_selector);
    TID(std::string img_path, bool crop_selector, Point crop_size);
    void init();

    ulong** _img_data;
    Point _img_size;

    std::string _img_path;
    Point _size;

    bool _crop_selector;
    int _last_crop;
    int _max_crops;
    int _random_hop;
    int _shift_step;
    Point _crop_size;

    static int prime_numbers[145]; 

    std::list<ulong**> _crops;

    int loadNextCrops(int n, int& totalRead, int& theEnd, int(* checkData)(ulong**,Point,void*), void* vp);
    void choose_random_hop(int total_crops);

};

int TID::prime_numbers = {83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541,547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,641,643,647,653,659,661,673,677,683,691,701,709,719,727,733,739,743,751,757,761,769,773,787,797,809,811,821,823,827,829,839,853,857,859,863,877,881,883,887,907,911,919,929,937,941,947,953,967,971,977,983,991,997};

#endif
