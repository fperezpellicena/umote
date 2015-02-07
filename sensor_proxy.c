#include <xc.h>

#include "sensor_proxy.h"
#include "sht/include/sht.h"

static void EnableSHT11(void);
static void MeasureSHT11(void);
static void DisableSHT11(void);

static void MeasureCO2(void);
static void DisableCO2(void);

void MeasureSensors(void) {
    EnableSHT11();
    MeasureSHT11();
    DisableSHT11();
    MeasureCO2();
    DisableCO2();
}

void EnableCO2(void) {
    TRISCbits.TRISC4 = 0;
    PORTCbits.RC4 = 1;
}

static void MeasureCO2(void) {

}

static void DisableCO2(void) {
    PORTCbits.RC4 = 0;
}

static void EnableSHT11(void) {
    TRISAbits.TRISA0 = 0;
    PORTAbits.RA0 = 1;
}

static void MeasureSHT11(void) {
    ShtData data;
    Sht11_init();
    Sht11_measure(&data);
}

static void DisableSHT11(void) {
    PORTAbits.RA0 = 0;
}
