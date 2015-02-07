#include "sensor_proxy.h"
#include "network_proxy.h"

void interrupt handleInterrupt(void) {
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
