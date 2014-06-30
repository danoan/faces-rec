#include "../headers/subwindowGenerator.h"

SubwindowGenerator::SubwindowGenerator(std::string img_path,Point wr,ulong shift_step){
	Point img_size;
	loadImage(NULL,img_path.c_str(),&img_size);

	_img_size = img_size;

	_wr = wr;
	_shift_step = shift_step;

	// printf("%lu %lu %lu %lu\n",img_size.y,img_size.x,wr.y,wr.x);

	int min_mask[2]={3,3};	
	_ce0 = fmin( ( img_size.x*0.2/_wr.x ), ( img_size.y*0.2/_wr.y ) );
	if(_ce0<0.5){
		_ce0 = 0.5;
	}

	if(Config::DETECTOR_SCAN_WINDOW_FACTOR!=0.0){
		_ce0=Config::DETECTOR_SCAN_WINDOW_FACTOR;	
	}
	
	 
	_ce_max = fmin( ( img_size.x/_wr.x ), ( img_size.y/_wr.y ) );
}

std::vector<Subwindow> SubwindowGenerator::generateSubwindows(int ng){
	int cur_ng = 0;
	int cursor = 0;

	std::vector<Subwindow> lista;

	while(cur_ng<ng){		
		double ce = fixed_factor(cur_ng,_ce0,_ce_max);		

		Point subwindow_size = computeSubwindowSize(_wr,ce);
		if( ( subwindow_size.x > _img_size.x ) || ( subwindow_size.y > _img_size.y ) ){
			break;
		}		

		// printf("GEN %f %lu %lu\n",ce,subwindow_size.y,subwindow_size.x);

		int max_y_pos = _img_size.y - subwindow_size.y + 1;
		int max_x_pos = _img_size.x - subwindow_size.x + 1;

		int y,x;
		for(y=0;y<max_y_pos;y+=_shift_step){
			for(x=0;x<max_x_pos;x+=_shift_step){
				lista.push_back( Subwindow(x,y,_wr,ce,cur_ng) );
			}
		}

		if(ce==_ce_max){
			break;
		}

		cur_ng+=1;
	}	

	return lista;
}

double fixed_factor(int ng, double ce0, double cef){
    double factor = ce0*pow(1.25,ng);
    if(factor>cef){
        factor=cef;
    }

    return factor;
};
