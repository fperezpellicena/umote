#include "sensor_proxy.h"
#include "sht/include/sht.h"
#include "adc/include/averaged_adc.h"

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
static void ClearCO2SensorTimerInterrupt(void);

static ShtData shtData;
static uint16_t co2;

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
    TRISCbits.TRISC5 = 0;
    PORTCbits.RC5 = 1;
}

void StartCO2SensorDelayed(void) {
//    OpenTimer0(TIMER_CONFIG);
    T0CONbits.TMR0ON = 0;
    T0CONbits.T08BIT = 1; // 16 bit mode
    T0CONbits.T0CS = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS = 6; // Prescale x128
    TMR0H = 0;
    TMR0L = 0;
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    INTCON2bits.T0IP = 0;
    T0CONbits.TMR0ON = 1;
}

static void StopCO2SensorDelayed(void) {
    T0CONbits.TMR0ON = 0;
    INTCONbits.TMR0IE = 0;
}

BOOL MustEnableCO2Sensor(void) {
    if (CO2SensorTimerOverflow()) {
        ClearCO2SensorTimerInterrupt();
        StartCO2SensorDelayed();
        overflowCount++;
    }
    if (overflowCount == OVF_RATE) {
        overflowCount = 0;
        return true;
    }
    return false;
}

static BOOL CO2SensorTimerOverflow(void) {
    return INTCONbits.T0IF && INTCONbits.T0IE;
}

static void ClearCO2SensorTimerInterrupt(void) {
     INTCONbits.TMR0IF = 0;
}

static void MeasureCO2(void) {
    AdcInit();
    AdcConvert(ADC_CH10, &co2);
    AdcClose();
}

static void DisableCO2(void) {
    PORTCbits.RC5 = 0;
}

static void EnableSHT11(void) {
    TRISAbits.TRISA5 = 0;
    PORTAbits.RA5 = 1;
}

static void MeasureSHT11(void) {
    Sht11_init();
    Sht11_measure(&shtData);
}

static void DisableSHT11(void) {
    PORTAbits.RA5 = 0;
}
