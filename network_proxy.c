#include "network_proxy.h"

BOOL NetworkAwake(void) {
    return INTCONbits.INT0IF && INTCONbits.INT0IE;
}

void ClearNetworkInterrupt(void) {
    INTCONbits.INT0IF = 0;
}

void SendMeasures(void) {
    
}

