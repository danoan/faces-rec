#include "../headers/cpuManager.h"

void* cpuThread(void* vp){
    elem_params* ep = (elem_params*) vp;

    TableItem partialBest;
    for(int i=ep->from;i<ep->to;i++){
        // printf("GO %d\n", i);
        partialBest = ep->t->_ct[ep->thread_number]->getBestTableItem( (ep->t->_facesFactory._facesFeatures)[i], ep->t->_tsm->trs() );
        // printf("STOP\n");

        if(partialBest._error<ep->best._error){
            ep->best = partialBest;
        }   
    }

    // printf("THE END\n");

    pthread_exit(NULL);

}

void CPUManager::createThreads(GPUBuffer* b, Trainer* t, elem_params* ep){
    int length = b->_to - b->_from + 1;
    int factor = length/CPU_THREADS + 1;

    
    for(int i=0;i<CPU_THREADS;i++){
        
        ep[i].thread_number = i;
        ep[i].best = TableItem(0,0,FACE,1.0,1);
        ep[i].t = t;
        ep[i].from = i*factor;
        ep[i].to = (i+1)*factor;
        if(ep[i].to>b->_to) ep[i].to=b->_to;

        pthread_create(&(_threads[i]), NULL, cpuThread, &ep[i]);
    }
}

TableItem CPUManager::consumeBuffer(GPUBuffer* b, Trainer* t){
	elem_params ep[CPU_THREADS];
    createThreads(b,t,ep);
    for(int i=0;i<CPU_THREADS;i++){
        pthread_join(_threads[i],NULL);
    }

    TableItem theBest = ep[0].best;
    for(register int t=1;t<THREADS_NUMBER;t++){
        if(ep[t].best._error < theBest._error){
            theBest = ep[t].best;
        }
    }    

    return theBest;
}
