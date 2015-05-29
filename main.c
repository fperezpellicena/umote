#include "bsp.h"
#include "interrupts.h"
#include "sensor_proxy.h"
#include "xbee/../digi_api.h"

static void Idle(void);

int main() {
    BspInit();
    XBeeInit();
    SensorProxyInit();
    EnableInterrupts();
    while (1) {
        //        Idle();
    }
    return 0;
}

/**
 * In RC_IDLE mode, the CPU is disabled but the peripherals continue to be
 * clocked from the internal oscillator block.
 * This mode allows for controllable power conservation during Idle periods.
 * From RC_RUN, this mode is entered by setting the IDLEN bit and executing
 * a SLEEP instruction.
 * If the device is in another Run mode, first set IDLEN, then clear the SCS
 * bits and execute SLEEP.
 * When the clock source is switched to the INTOSC block, the primary oscillator
 * is shutdown and the OSTS bit is cleared.
 * When a wake event occurs, the peripherals continue to be clocked from the
 * internal oscillator block.
 * After a wake event, the CPU begins executing code being clocked by the INTRC.
 * The IDLEN and SCS bits are not affected by the wake-up.
 * The INTRC source will continue to run if either the WDT or the FSCM is enabled.
 */
static void Idle(void) {
    // FIXME ¿Should check for timer 1 status?
    OSCCONbits.IDLEN = 1;
    // If RC_RUN is not enabled(SCS = '11') then set SCS = 0x11
    if (OSCCONbits.SCS != 3) {
        OSCCONbits.SCS = 3;
    }
    Sleep();
    Nop();
}
