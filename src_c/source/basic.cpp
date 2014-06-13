#include "../headers/basic.h"

struct timeval stop,start;

Subwindow::Subwindow(int x, int y, Point wr, double ce, int ng){
    _x = x;
    _y = y;
    
    _size = computeSubwindowSize(wr,ce);
    
    _cur_ng = ng;
    _ce = ce;
}

void startClock(){
    gettimeofday(&start,NULL);
}

void stopClock(std::string testName){
    gettimeofday(&stop,NULL);
    ulong seg = (stop.tv_sec-start.tv_sec); 
    ulong useg = (stop.tv_usec-start.tv_usec);  

    printf("TEMPO EXECUCAO (%s): %lu.%lu seg\n",testName.c_str(), seg,useg);        
}

FilePrint* Logger::logger = new FilePrint("log_d.txt");
FilePrint* Logger::error = new FilePrint("error_d.txt");
FilePrint* Logger::debug = new FilePrint("debug_d.txt");
FilePrint* Logger::feat_anal = new FilePrint("feat_anal_d.txt");

