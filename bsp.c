#include "bsp.h"

void BspInit(void) {
    // All analog inputs to ADC are by default digital
    ANCON0 = 0xFF;
    ANCON1 = 0xFF;
    // INTOSC drives clock directly
    OSCCONbits.IRCF = 7;
}
