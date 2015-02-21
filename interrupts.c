#include "bsp.h"
#include "sensor_proxy.h"
#include "network_proxy.h"

static volatile uint8_t count = 0;

void EnableInterrupts(void) {
    // Clear peripheral priorities
    IPR1 = 0x00;
    IPR2 = 0x00;
    IPR3 = 0x00;
    // Clear interrupt flags
    PIR1 = 0x00;
    PIR2 = 0x00;
    PIR3 = 0x00;
    // All analog inputs to ADC are by default digital
    ANCON0 = 0xFF;
    ANCON1 = 0xFF;
    // Configure interrupts
    RCONbits.IPEN = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIEH = 1; /* Enable interrupts*/
    INTCONbits.GIEL = 1; /* Enable interrupts*/
}

// Configure INT0 & enable it

void EnableNetworkInterrupt(void) {
    // INT0 rising edge mode
    INTCON2bits.INTEDG0 = 1;
    // Disable pullup
    INTCON2bits.RBPU = 0;
    // Enable INT0
    INTCONbits.INT0IE = 1;
    // Clear INT0 flag
    INTCONbits.INT0IF = 0;
    // Configure RB0 as input
    TRISBbits.TRISB0 = 1;
}

// Interrupt handler

void interrupt HandleInterrupt(void) {
    if (NetworkAwake()) {
        // Clear network interrupt
        ClearNetworkInterrupt();
        // Gather measurements
        MeasureSensors();
        // Send measurements
        SendMeasures();
        // Prepare CO2 sensor
        StartCO2SensorDelayed();
    }
}

void interrupt low_priority HandleLowPriorityInterrupt(void) {
    if (MustEnableCO2Sensor()) {
        // Enable CO2 sensor
        EnableCO2Sensor();
    }
}
