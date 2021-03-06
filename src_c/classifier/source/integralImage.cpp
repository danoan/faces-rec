#include "../headers/integralImage.h"

void IntegralImage::init(ulong**& data,Point size, bool copy){
	if(copy){
		_data = (ulong**) malloc(sizeof(ulong*)*size.y);
		for(int i=0;i<size.y;i++){
			_data[i] = (ulong*) malloc(sizeof(ulong)*size.x);
			for(int j=0;j<size.x;j++){
				_data[i][j] = data[i][j];
			}
		}		
	}else{
		_data=data;
	}	
	
}

IntegralImage::IntegralImage(std::string img_path):_copy(true){
	int y,x;
	ulong line_sum;
	loadImage(&(_data),img_path.c_str(),&(_size));
	// printf("INT IM %d %d\n",_size.y,_size.x);
}

IntegralImage::IntegralImage(ulong**& data,Point size, bool copy):_size(size),_copy(copy){
	init(data,size,copy);
}

IntegralImage::IntegralImage(ulong**& data,Point size):_size(size),_copy(true){
	init(data,size,true);
}

inline int IntegralImage::outsideLimits(Point& p){
	return (p.y>=_size.y) || (p.y<0) || (p.x>=_size.x) || (p.x<0);
}

long IntegralImage::computeBlock(std::vector<MaskBlock> &b, Point& location){
	long block_sum = 0;
	Point pa,pb,pc,pd;
	Point points[4];

	for(register int i=0;i<b.size();++i){
		

		pa.y = location.y + b[i]._points[0].y - 1;
		pa.x = location.x + b[i]._points[0].x - 1;

		pb.y = location.y + b[i]._points[1].y - 1;
		pb.x = location.x + b[i]._points[1].x;

		pc.y = location.y + b[i]._points[2].y;
		pc.x = location.x + b[i]._points[2].x - 1;

		pd.y = location.y + b[i]._points[3].y;
		pd.x = location.x + b[i]._points[3].x;						


		points[0] = pa;
		points[1] = pb;
		points[2] = pc;
		points[3] = pd;

		// printf("%ld (%lu %lu) [ (%lu %lu) (%lu %lu) (%lu %lu) (%lu %lu) ]\n",getFromData(points),location.x,location.y,pa.y,pa.x,pb.y,pb.x,pc.y,pc.x,pd.y,pd.x);
		// printf("%ld (%lu %lu)",b[i]._points[0].y,b[i]._points[0].x);
		
		block_sum+=getFromData(points);		
	}

	return block_sum;
}

long IntegralImage::getFromData(Point* points){
	long sum[4]={0,0,0,0};
	Point p;

	for(register int i=0;i<4;++i){
		p = points[i];
		if(outsideLimits(p)){
			sum[i] = 0;
		}else{
			sum[i] = _data[p.y][p.x];
		}
	}

	return (sum[0]+sum[3]-sum[2]-sum[1]);
}

ulong IntegralImage::filter(FeatureMask &fm){
	if( (fm._mask._size.x > _size.x) || (fm._mask._size.y > _size.y) ){
		return 0;
	}

	// printf("%d\n", abs( computeBlock(fm._mask._black,fm._location)  - computeBlock(fm._mask._white,fm._location) ) );

	return abs( computeBlock(fm._mask._black,fm._location)  - computeBlock(fm._mask._white,fm._location) );
}
