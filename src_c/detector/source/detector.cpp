#include "../headers/detector.h"

Detector::Detector(int ng, Point wr, Point ref_mask, int shift_step){
	_ng = ng;
	_wr = wr;
	_ref_mask = ref_mask;
	_shift_step = shift_step;
}

int Detector::detectFaces(ClassifierInterface* ci, const std::string img_path, double ac, int draw){	
	SubwindowGenerator sg(img_path,_wr,_shift_step);
	IntegralImage ii(img_path);

	ulong total_sw,total_faces;
	std::vector<Subwindow> lista = sg.generateSubwindows(_ng);

	total_faces=0;
	total_sw = lista.size();

	int res;		
	std::vector<int*> faces_boxes;
	std::vector<int*> scenes_boxes;
	for(register int i=0;i<lista.size();++i){		
		// printf("NEW SUBWINDOW\n");
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

	if(draw==1){
		int** boxes_array = (int**) malloc(faces_boxes.size()*sizeof(int*));
		for(register int i=0;i<faces_boxes.size();i++){
			boxes_array[i] = faces_boxes[i];
		}
		drawRectangles(img_path.c_str(),faces_boxes.size(),boxes_array,"green");
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

int Detector::detectFaces(ClassifierInterface* ci, const std::string img_path, int draw){
	return detectFaces(ci,img_path,-1.0,draw);	
}

