#include "register.h"
#include <xc.h>

void RegisterLockIO(void) {
    EECON2 = 0x55;
    EECON2 = 0xAA;
    PPSCONbits.IOLOCK = 0;
}

void RegisterUnLockIO(void) {
    EECON2 = 0x55;
    EECON2 = 0xAA;
    PPSCONbits.IOLOCK =1;
}

