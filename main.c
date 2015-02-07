#include <stdint.h>
#include <stdbool.h>

#include <xc.h>
#include "sensor_proxy.h"

#pragma config WDTEN = OFF          //WDT disabled (enabled by SWDTEN bit)
#pragma config WDTPS = 1024         //1:1024 (4 seconds)
#pragma config DSWDTOSC = INTOSCREF //DSWDT uses INTOSC/INTRC as clock
#pragma config DSWDTEN = ON         //Enabled
#pragma config DSWDTPS = K524       //9 minutes

#pragma config STVREN = ON          //stack overflow/underflow reset enabled
#pragma config XINST = OFF          //Extended instruction set disabled
#pragma config CP0 = OFF            //Program memory is not code-protected

#pragma config OSC = INTOSCPLL      //Internal OSC(8mhz), PLL enabled by soft
#pragma config PLLDIV = 2           //Divide By 2(4 Mhz) to generate 96Mhz
#pragma config CPUDIV = OSC4_PLL6   //CPU system clock divide 6

#pragma config FCMEN = OFF          //Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF           //Two-Speed Start-up disabled

#pragma config RTCOSC = INTOSCREF   //RTCC uses INTRC
#pragma config DSBOREN = OFF        //Zero-Power BOR disabled in Deep Sleep

#pragma config IOL1WAY = OFF        //IOLOCK bit can be set and cleared
#pragma config MSSP7B_EN = MSK7     //7 Bit address masking
#pragma config WPFP = PAGE_1        //Write Protect Program Flash Page 0
#pragma config WPEND = PAGE_0       //Start protection at page 0
#pragma config WPCFG = OFF          //Write/Erase last page protect Disabled
#pragma config WPDIS = OFF          //WPFP[5:0], WPEND, and WPCFG bits ignored


#define OVF_RATE    9

static uint8_t overflowCount;

void Idle(void);
BOOL NetworkAwake(void);
BOOL TimerOverflow(void);

int main() {
    // Configure INT0 & enables it
    OpenRB0INT(PORTB_CHANGE_INT_ON | RISING_EDGE_INT | PORTB_PULLUPS_OFF);
    while (1) {
        Idle();
    }
    return 0;
}

void interrupt handleInterrupt(void) {
    if (NetworkAwake()) {
        // Take measurements
        MeasureSensors();
        // Configure and run timer 0
        OpenTimer0(T0_16BIT | T0_SOURCE_INT | TIMER_INT_ON | T0_PS_1_256);
    } else if (TimerOverflow()) {
        // Clear interrupt flag
        OpenTimer0(T0_16BIT | T0_SOURCE_INT | TIMER_INT_ON | T0_PS_1_256);
        if (overflowCount++ == OVF_RATE) {
            // Stop timer 0
            CloseTimer0();
            // Enable CO2 sensor heater (RC4)
            EnableCO2();
        };
    }
}

BOOL NetworkAwake(void) {
    return INTCONbits.INT0IF && INTCONbits.INT0IE;
}

BOOL TimerOverflow(void) {
    return INTCONbits.T0IF && INTCONbits.T0IE;
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
void Idle(void) {
    // FIXME ¿Should check for timer 1 status?
    OSCCONbits.IDLEN = 1;
    // If RC_RUN is not enabled(SCS = '11') then set SCS = 0x11
    if (OSCCONbits.SCS != 3) {
        OSCCONbits.SCS = 3;
    }
    Sleep();
    Nop();
}

