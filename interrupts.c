#include "bsp.h"
#include "interrupts.h"
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
    // Configure interrupts
    RCONbits.IPEN = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIEH = 1; /* Enable interrupts*/
    INTCONbits.GIEL = 1; /* Enable interrupts*/
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
    } else if (MustToggleLampPulse()) {
        ToggleLampPulse();
    }
}
