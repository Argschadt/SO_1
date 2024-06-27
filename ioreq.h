#include "osPRNG.h"
#define PROB_OF_IO_REQ 10
#define PROB_OF_IO_TERM 4
int IOReq() {
    if( osPRNG() % PROB_OF_IO_REQ == 0 )
        return 1;
    else
        return 0;
}

int IOTerm(){
    if( osPRNG() % PROB_OF_IO_TERM == 0 )
        return 1;
    else
        return 0;
}