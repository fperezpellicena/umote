#include "bsp.h"

void BspInit(void) {
    OSCCONbits.IRCF = 7;
}
