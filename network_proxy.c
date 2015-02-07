#include "network_proxy.h"


BOOL NetworkAwake(void) {
    return INTCONbits.INT0IF && INTCONbits.INT0IE;
}

void SendMeasures(void) {
    
}

