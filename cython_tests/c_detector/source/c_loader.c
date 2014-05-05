#include "../headers/c_loader.h"

void printC_Block(C_Block* b, ulong len){
	int i,j;
	for(i=0;i<len;i++){
		printf("[ ");
		for(j=0;j<4;j++){
			printf("(%lu,%lu)",b[i].points[j].x,b[i].points[j].y);	
		}
		printf("] W: %lu H: %lu\n",b[i].w,b[i].h);		
	}	
	printf("\n");
}

void printC_FeatureMask(C_FeatureMask fm){
	printf("\tMask:\n");
	printf("\t\tWhite:\n");
	printC_Block(fm.mask.white,fm.mask.len_white);

	printf("\t\tBlack:\n");
	printC_Block(fm.mask.white,fm.mask.len_white);	

	printf("Location: (%lu, %lu)\nOriginal_Location:(%lu,%lu)\nOriginal_Size:(%lu,%lu)\n",fm.location.x,fm.location.y,fm.__original_location.x,fm.__original_location.y,fm.__original_size.x,fm.__original_size.y);
}

void printC_Hypothesy(C_Hypothesy h){
	printf("Threshold: %lu\nDirection: %ld\nAlpha: %f\nFeatureMask:\n",h.threshold,h.direction,h.alpha);
	printC_FeatureMask(h.fm);
}

void printC_Classifier(C_Classifier cl){
	int i;
	printf("Print Classificador\n\n Final:%lu\n Ardis:(%lu,%lu)\n\n",cl.final,cl.ardis.y,cl.ardis.x);
	for(i=0;i<cl.len_hypothesis;i++){
		printf("\n\nHypothesis (%d-%lu)\n\n",i,cl.len_hypothesis);
		printC_Hypothesy(cl.hypothesis[i]);		
	}	
}

C_Classifier loadC_Classifier(const char* filename){
	C_Classifier cl;
	
	FILE* f = fopen(filename,"rb");
	
	fread(&cl.final,sizeof(ulong),1,f);
	fread(&cl.ardis,sizeof(C_Point),1,f);
	fread(&cl.len_hypothesis,sizeof(ulong),1,f);

	cl.hypothesis = (C_Hypothesy*) malloc(sizeof(C_Hypothesy)*cl.len_hypothesis);
	int i,j;
	for(i=0;i<cl.len_hypothesis;++i){
		C_Hypothesy* h = &(cl.hypothesis[i]);
		fread(&h->threshold,sizeof(ulong),1,f);
		fread(&h->direction,sizeof(long),1,f);
		fread(&h->alpha,sizeof(double),1,f);
		
		fread(&h->fm.__original_location,sizeof(C_Point),1,f);
		fread(&h->fm.__original_size,sizeof(C_Point),1,f);
		fread(&h->fm.location,sizeof(C_Point),1,f);

		fread(&h->fm.mask.maskType,sizeof(int),1,f);
		fread(&h->fm.mask.size,sizeof(C_Point),1,f);

		fread(&h->fm.mask.len_white,sizeof(ulong),1,f);
		h->fm.mask.white = (C_Block*) malloc(sizeof(C_Block)*h->fm.mask.len_white);

		for(j=0;j<h->fm.mask.len_white;++j){
			fread(&h->fm.mask.white[j].points,sizeof(C_Point),4,f);
			fread(&h->fm.mask.white[j].w,sizeof(ulong),1,f);
			fread(&h->fm.mask.white[j].h,sizeof(ulong),1,f);
		}

		fread(&h->fm.mask.len_black,sizeof(ulong),1,f);
		h->fm.mask.black = (C_Block*) malloc(sizeof(C_Block)*h->fm.mask.len_black);

		for(j=0;j<h->fm.mask.len_black;++j){
			fread(&h->fm.mask.black[j].points,sizeof(C_Point),4,f);
			fread(&h->fm.mask.black[j].w,sizeof(ulong),1,f);
			fread(&h->fm.mask.black[j].h,sizeof(ulong),1,f);
		}		

		h->fm.__original_mask = h->fm.mask;
	}

	fclose(f);

	return cl;
}
