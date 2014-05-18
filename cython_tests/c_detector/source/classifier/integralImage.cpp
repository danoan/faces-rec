#include "../../headers/classifier/integralImage.h"

IntegralImage::IntegralImage(std::string img_path){
	int y,x;
	ulong line_sum;
	loadImage(&(_data),img_path.c_str(),&(_size));
}

inline int IntegralImage::outsideLimits(Point p){
	return (p.y>=_size.y) || (p.y<0) || (p.x>=_size.x) || (p.x<0);
}

long IntegralImage::computeBlock(std::vector<MaskBlock> &b, Point location){
	long block_sum = 0;

	for(register int i=0;i<b.size();++i){
		Point pa,pb,pc,pd;

		pa.y = location.y + b[i]._points[0].y - 1;
		pa.x = location.x + b[i]._points[0].x - 1;

		pb.y = location.y + b[i]._points[1].y - 1;
		pb.x = location.x + b[i]._points[1].x;

		pc.y = location.y + b[i]._points[2].y;
		pc.x = location.x + b[i]._points[2].x - 1;

		pd.y = location.y + b[i]._points[3].y;
		pd.x = location.x + b[i]._points[3].x;						


		Point points[4] = {pa,pb,pc,pd};

		// printf("%ld (%lu %lu) [ (%lu %lu) (%lu %lu) (%lu %lu) (%lu %lu) ]\n",getFromData(points),location.x,location.y,pa.y,pa.x,pb.y,pb.x,pc.y,pc.x,pd.y,pd.x);
		// printf("%ld (%lu %lu)",b[i]._points[0].y,b[i]._points[0].x);
		
		block_sum+=getFromData(points);		
	}

	return block_sum;
}

long IntegralImage::getFromData(Point points[4]){
	long sum[4]={0,0,0,0};

	for(register int i=0;i<4;++i){
		Point p = points[i];
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

	// printf("%d\n",abs(black_sum-white_sum));

	return abs( computeBlock(fm._mask._black,fm._location)  - computeBlock(fm._mask._white,fm._location) );
}
