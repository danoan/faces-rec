#include <cstdio>
#include <cstdlib>

#include "../headers/basic.h"

#include "../headers/pyToC/libpytoc.h"
#include "../headers/classifier/libclassifier.h"
#include "../headers/feature/libfeature.h"
#include "../headers/detector/libdetector.h"

void testFactory(std::string name, FMF factory){
	ulong c=0;

	while(factory.hasNext()==1){
		factory.next();
		c++;
	}

	printf("%s: %lu\n",name.c_str(),c);	
}

void testeFMF(){
	std::vector<ulong> v = incrementList( 1.25, 3, 6, 50 );
	for(int i=0;i<v.size();i++){
		printf("%lu\n",v[i]);
	}

	Point ardis;
	ardis.y = 64;
	ardis.x = 64;

	MaskTwoHorizontalFactory m2h = MaskTwoHorizontalFactory(ardis,1,1,1.25,8,8);
	MaskTwoVerticalFactory m2v =  MaskTwoVerticalFactory(ardis,1,1,1.25,8,8);
	MaskThreeHorizontalFactory m3h = MaskThreeHorizontalFactory(ardis,1,1,1.25,8,8);
	MaskThreeVerticalFactory m3v = MaskThreeVerticalFactory(ardis,1,1,1.25,8,8);

	MaskDiagonalFactory md = MaskDiagonalFactory(ardis,1,1,1.25,8,8);
	
	testFactory("TwoHorizonal",m2h);
	testFactory("TwoVertical",m2v);
	testFactory("ThreeHorizonal",m3h);
	testFactory("ThreeVertical",m3v);
	testFactory("Diagonal",md);

}

void teste_integral(){
	std::string img_filename = "/home/daniel/Projects/faces-rec/dataset/test_images/featmastertestcase.pgm" ;

	IntegralImage ii(img_filename.c_str());

	for(register int i=0;i<4;++i){
		for(register int j=0;j<4;++j){
			printf("%lu ",ii.data()[i][j]);		
		}
		printf("\n");
	}

	Point ardis;
	ardis.y = 64;
	ardis.x = 64;

	MaskTwoHorizontalFactory m2hf(ardis);
	MaskTwoVerticalFactory m2vf(ardis);
	MaskThreeHorizontalFactory m3hf(ardis);
	MaskThreeVerticalFactory m3vf(ardis);
	MaskDiagonalFactory mdf(ardis);

	FeatureMask fm = m2hf.next();
	printf("M2HF: %lu\n",ii.filter( fm ) );

	fm = m2vf.next();
	printf("M2VF: %lu\n",ii.filter( fm ) );

	fm = m3hf.next();
	printf("M3HF: %lu\n",ii.filter( fm ) );

	fm = m3vf.next();
	printf("M3VF: %lu\n",ii.filter( fm ) );	

	fm = mdf.next();
	printf("MDF: %lu\n",ii.filter( fm ) );	


	// im.putdata([10, 20, 30, 50,
	// 			   60, 70, 80, 90,
	// 			   100,110,120,130,
	// 			   140,150,160,170])	

	// assert fm.m2h_values[0] == 20
	// assert fm.m2v_values[0] == 100
	// assert fm.m3h_values[0] == 90
	// assert fm.m3v_values[0] == 110
	// assert fm.md_values[0] == 0	
}

void teste_deteccao(Classifier cl,std::string img_filename){
	startClock();

	Point wr;
	wr.x=64;
	wr.y=64;

	Point ref_mask;
	ref_mask.x=8;
	ref_mask.y=8;	

	Detector d(cl,6, wr, ref_mask, 1, 0.5);
	d.detectFaces(img_filename.c_str());

	stopClock("Deteccao");
}


int main(int argc, char* argv[]){
	std::string cl_filename = "/home/daniel/Projects/faces-rec/classifiers/10000/novos/classifier_10000_(45-100).pk";
	std::string img_filename = "/home/daniel/Projects/faces-rec/dataset/test_images/seinfeld.pgm" ;

	C_Classifier c_cl = loadC_Classifier(cl_filename.c_str());	
	Classifier cl = load_Classifier(cl_filename.c_str());

	printf("%d %d %d\n",c_cl.hypothesis[0].fm.mask.maskType,c_cl.hypothesis[1].fm.mask.maskType,c_cl.hypothesis[2].fm.mask.maskType);

	// teste_deteccao(cl,img_filename);
	// teste_integral();
	testeFMF();

	return 0;
}



