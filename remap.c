#include "register.h"

/* Remap I/O pin with function */
void RegisterRemap(far uint8_t* rpin, uint8_t func) {
    RegisterUnLockIO();
    *rpin = func;
    RegisterLockIO();
}
