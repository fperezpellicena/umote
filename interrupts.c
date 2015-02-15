#include "sensor_proxy.h"
#include "network_proxy.h"

// Configure INT0 & enable it
void EnableNetworkInterrupt(void) {
    OpenRB0INT(PORTB_CHANGE_INT_ON | RISING_EDGE_INT | PORTB_PULLUPS_OFF);
}

// Interrupt handler
void interrupt HandleInterrupt(void) {
    if (NetworkAwake()) {
        // Gather measurements
        MeasureSensors();
        // Send measurements
        SendMeasures();
        // Prepare CO2 sensor
        StartCO2SensorDelayed();
    } else if (MustEnableCO2Sensor()) {
        // Enable CO2 sensor
        EnableCO2Sensor();
    }
}
