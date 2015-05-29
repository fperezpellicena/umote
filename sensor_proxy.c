#include "sensor_proxy.h"
#include "sht/include/sht.h"
#include "adc/include/averaged_adc.h"
#include "irca/irca.h"
#include <math.h>

#define TIMER_CONFIG                    T0_16BIT | T0_SOURCE_INT | TIMER_INT_ON | T0_PS_1_256
#define ENABLE_DELAYED_OVF_RATE         9
#define PULSE_DELAYED_OVF_RATE          4

volatile static uint8_t enableDelayedCounter;
volatile static uint8_t pulseDelayedCounter;

static void PowerUpCO2Sensor(void);
static void PowerDownCO2Sensor(void);
static void PowerUpSHT11(void);
static void PowerDownSHT11(void);

static void DisableCO2(void);
static void StopCO2SensorDelayed(void);

static BOOL CO2SensorEnableTimerOverflow(void);
static void ClearCO2SensorEnableTimerInterrupt(void);

static BOOL CO2SensorPulseTimerOverflow(void);
static void ClearCO2SensorPulseTimerInterrupt(void);

static void LMP91051Init(void);

ShtData shtData;
IrcAData ircaData;

void SensorProxyInit(void) {
    Sht11Init();
    IrcaInit();
}

void MeasureSensors(void) {
    // SHT11 sequence
    PowerUpSHT11();
    Sht11Measure(&shtData);
    PowerDownSHT11();
    // IRCA sequence
    ircaData.tmp = 0;
    IrcaMeasure(&ircaData);
    DisableCO2();
}

static void PowerUpCO2Sensor(void) {
    TRISCbits.TRISC1 = 0;
    PORTCbits.RC1 = 1;
}

static void PowerDownCO2Sensor(void) {
    PORTCbits.RC1 = 0;
}

static void PowerUpSHT11(void) {
    TRISAbits.TRISA5 = 0;
    PORTAbits.RA5 = 1;
}

static void PowerDownSHT11(void) {
    PORTAbits.RA5 = 0;
}

/**
 * Power up CO2 circuit and enable lamp trigger
 */
void EnableCO2Sensor(void) {
    StopCO2SensorDelayed();
    PowerUpCO2Sensor();
    IrcaEnableLamp();
}

static void DisableCO2(void) {
    PowerDownCO2Sensor();
    IrcaDisableLamp();
}

void StartCO2SensorDelayed(void) {
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
    if (CO2SensorEnableTimerOverflow()) {
        ClearCO2SensorEnableTimerInterrupt();
        StartCO2SensorDelayed();
        enableDelayedCounter++;
    }
    if (enableDelayedCounter == ENABLE_DELAYED_OVF_RATE) {
        enableDelayedCounter = 0;
        return true;
    }
    return false;
}

void ToggleLampPulse(void) {
    IRCA1_PULSE = !IRCA1_PULSE;
}

BOOL MustToggleLampPulse(void) {
    if (CO2SensorPulseTimerOverflow()) {
        pulseDelayedCounter++;
    }
    ClearCO2SensorPulseTimerInterrupt();
    if (pulseDelayedCounter == PULSE_DELAYED_OVF_RATE) {
        pulseDelayedCounter = 0;
        return true;
    }
    return false;
}

static BOOL CO2SensorPulseTimerOverflow(void) {
    return PIR1bits.TMR1IF && PIE1bits.TMR1IE;
}

static void ClearCO2SensorPulseTimerInterrupt(void) {
    PIR1bits.TMR1IF = 0;
}

static BOOL CO2SensorEnableTimerOverflow(void) {
    return INTCONbits.T0IF && INTCONbits.T0IE;
}

static void ClearCO2SensorEnableTimerInterrupt(void) {
    INTCONbits.TMR0IF = 0;
}
