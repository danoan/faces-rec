#include "../../headers/util/basic.h"

struct timeval stop,start;
FilePrint* Logger::logger;
FilePrint* Logger::error;
FilePrint* Logger::debug;
FilePrint* Logger::feat_anal;

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

void Logger::init(std::string folder){
    startClock();
    char time_str[12];
    sprintf(time_str,"%lu",start.tv_sec);

    Logger::logger = new FilePrint(folder + "/log_" + time_str + ".txt");
    Logger::error = new FilePrint(folder + "/error_" + time_str + ".txt");
    Logger::debug = new FilePrint(folder + "/debug_" + time_str + ".txt");
    Logger::feat_anal = new FilePrint(folder + "/feat_anal_" + time_str + ".txt");        
}


