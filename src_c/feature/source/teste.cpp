#include <cstdio>

#include "../../util/headers/plainWriter.h"
#include "../headers/libfeature.h"

int teste_fm_plain_writer(){
	FacesFeatureFactory fff;
	
	PlainWriter<ulong> pw;
	FeatureMask::asPlainVector(pw,fff._facesFeatures[0]);
	FeatureMask::asPlainVector(pw,fff._facesFeatures[12000]);
	
	FeatureMask fm0 = FeatureMask::fromPlainVector(pw.moveBegin());
	FeatureMask fm1 = FeatureMask::fromPlainVector(pw);
	
	std::cout << fff._facesFeatures[0];	
	printf("INICIO\n");
	std::cout << fm0;
	std::cout << fm1;
}

int main(int argc, char* argv[]){
	teste_fm_plain_writer();
}
