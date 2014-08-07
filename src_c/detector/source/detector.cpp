#include "../headers/detector.h"

Detector::Detector(int ng, Point wr, Point ref_mask, int shift_step){
	_ng = ng;
	_wr = wr;
	_ref_mask = ref_mask;
	_shift_step = shift_step;
}

int Detector::detectFaces(ClassifierInterface* ci, const std::string img_path, int draw){
	return detectFaces(ci,img_path,-1.0,draw,0);	
}


int Detector::detectFaces(ClassifierInterface* ci, const std::string img_path, double ac, int draw){
	return detectFaces(ci, img_path, ac, draw, 0);
}

int Detector::detectFaces(ClassifierInterface* ci, const std::string img_path,int draw, int save){
	return detectFaces(ci, img_path, -1.0, draw, save);
}

int Detector::detectFaces(ClassifierInterface* ci, const std::string img_path, double ac, int draw, int save){	
	SubwindowGenerator sg(img_path,_wr,_shift_step);
	IntegralImage ii(img_path);

	startClock();
	ulong total_sw,total_faces;
	std::vector<Subwindow> lista = sg.generateSubwindows(_ng);
	stopClock("Generating Windows");

	total_faces=0;
	total_sw = lista.size();

	int res;		
	int cur_gen=-1;
	std::vector<int*> faces_boxes;
	std::vector<int*> scenes_boxes;
	startClock();
	for(register int i=0;i<lista.size();++i){		
		// printf("NEW SUBWINDOW\n");

		if(cur_gen!=lista[i]._cur_ng){
			ci->resize(lista[i]._ce);
			cur_gen = lista[i]._cur_ng;
		}

		if(ac<0){
			res = ci->isFace(ii,(lista[i]));
		}else{
			res = ci->isFace(ii,(lista[i]),ac);
		}

		if(res==1){						
			faces_boxes.push_back(new int[4]);
			lista[i].cropBox( faces_boxes[faces_boxes.size()-1] );		
			total_faces+=1;
		}else{
			scenes_boxes.push_back(new int[4]);
			lista[i].cropBox( scenes_boxes[scenes_boxes.size()-1] );			
		}
	}
	stopClock("Detection Time");

	if(draw==1){
		int** boxes_array = (int**) malloc(faces_boxes.size()*sizeof(int*));
		for(register int i=0;i<faces_boxes.size();i++){
			boxes_array[i] = faces_boxes[i];
		}
		std::string save_path = Config::PROJECT_PATH + "/analysis/detector_output/img_det.pgm";
		drawRectangles(img_path.c_str(),faces_boxes.size(),boxes_array,"white", save, save_path.c_str() );
		free(boxes_array);

		// int** boxes_array = (int**) malloc(scenes_boxes.size()*sizeof(int*));
		// for(register int i=0;i<scenes_boxes.size();i++){
		// 	boxes_array[i] = scenes_boxes[i];
		// }
		// drawRectangles(img_path.c_str(),scenes_boxes.size(),boxes_array,"red");
		// free(boxes_array);		
	}

	printf("\nSW: %lu FACES: %lu\n",total_sw,total_faces);
	return total_faces;
}
