#include "sensor_proxy.h"
#include "sht/include/sht.h"

#define TIMER_CONFIG        T0_16BIT | T0_SOURCE_INT | TIMER_INT_ON | T0_PS_1_256
#define OVF_RATE            9

static uint8_t overflowCount;

static void EnableSHT11(void);
static void MeasureSHT11(void);
static void DisableSHT11(void);

static void MeasureCO2(void);
static void DisableCO2(void);
static void StopCO2SensorDelayed(void);
static void EnableCO2(void);
static BOOL CO2SensorTimerOverflow(void);

static ShtData shtData;

void MeasureSensors(void) {
    EnableSHT11();
    MeasureSHT11();
    DisableSHT11();
    MeasureCO2();
    DisableCO2();
}

void EnableCO2Sensor(void) {
    StopCO2SensorDelayed();
    EnableCO2();
}

static void EnableCO2(void) {
    TRISCbits.TRISC4 = 0;
    PORTCbits.RC4 = 1;
}

void StartCO2SensorDelayed(void) {
    OpenTimer0(TIMER_CONFIG);
}

static void StopCO2SensorDelayed(void) {
    CloseTimer0();
}

BOOL MustEnableCO2Sensor(void) {
    if (CO2SensorTimerOverflow()) {
        StartCO2SensorDelayed();
        overflowCount++;
    }
    return overflowCount++ == OVF_RATE;
}

static BOOL CO2SensorTimerOverflow(void) {
    return INTCONbits.T0IF && INTCONbits.T0IE;
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
    Sht11_init();
    Sht11_measure(&shtData);
}

static void DisableSHT11(void) {
    PORTAbits.RA0 = 0;
}
