#include "network_proxy.h"
#include "sht.h"
#include "irca/irca.h"

extern ShtData shtData;
extern IrcA1Data ircaData;

BOOL NetworkAwake(void) {
    return INTCONbits.INT0IF && INTCONbits.INT0IE;
}

void ClearNetworkInterrupt(void) {
    INTCONbits.INT0IF = 0;
}

void SendMeasures(void) {
    // TODO Send data structs
}

