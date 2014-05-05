#include "../headers/detector.h"

Detector::Detector(Classifier cl, int ng, Point wr, Point ref_mask, int shift_step, double ac){
	_cl = cl;
	_ng = ng;
	_wr = wr;
	_ref_mask = ref_mask;
	_shift_step = shift_step;
	_ac = ac;
}

void Detector::detectFaces(std::string img_path){	
	SubwindowGenerator sg(img_path,_wr,_shift_step);
	IntegralImage ii(img_path);

	ulong total_sw,total_faces;
	std::vector<Subwindow> lista = sg.generateSubwindows(_ng);

	total_faces=0;
	total_sw = lista.size();

	int box[4];			
	for(register int i=0;i<lista.size();++i){
		if( _cl.isFace(ii,(lista[i]),_ac)==1 ){						
			lista[i].cropBox( box );
			total_faces+=1;
		}else{

		}
	}

	printf("\nSW: %lu FACES: %lu\n",total_sw,total_faces);
}
