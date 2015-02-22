#include "sensor_proxy.h"
#include "sht/include/sht.h"
#include "adc/include/averaged_adc.h"
#include "irca/irca.h"

#include <spi.h>

#define TIMER_CONFIG        T0_16BIT | T0_SOURCE_INT | TIMER_INT_ON | T0_PS_1_256
#define OVF_RATE            9
#define DEV_CONFIG_REG      (0x60)                   // FIXME PGA1 ON, PGA2 ON, CMN_Mode = 1.15V

static uint8_t overflowCount;

static void EnableSHT11(void);
static void MeasureSHT11(void);
static void DisableSHT11(void);

static void MeasureCO2(void);
static void DisableCO2(void);
static void StopCO2SensorDelayed(void);

static void PowerUpCO2Sensor(void);
static void PowerDownCO2Sensor(void);

static BOOL CO2SensorTimerOverflow(void);
static void ClearCO2SensorTimerInterrupt(void);

static void LMP91051Init(void);

static ShtData shtData;
static IrcA1Data irca1Data;

void SensorProxyInit(void) {
    Sht11_init();
    Irca1Init();
}

void MeasureSensors(void) {
    EnableSHT11();
    MeasureSHT11();
    DisableSHT11();
    MeasureCO2();
    DisableCO2();
}

/**
 * Power up CO2 circuit and enable lamp trigger
 */
void EnableCO2Sensor(void) {
    StopCO2SensorDelayed();
    PowerUpCO2Sensor();
    Irca1EnableLamp();
    LMP91051Init();
}

static void PowerUpCO2Sensor(void) {
    TRISCbits.TRISC5 = 0;
    PORTCbits.RC5 = 1;
}

 // Configure LMP registers via SPI
static void LMP91051Init(void) {
    OpenSPI(SPI_FOSC_4, MODE_00, SMPEND);
    WriteSPI(DEV_CONFIG_REG);
    // Debug
    uint8_t config = ReadSPI();
    if (config != DEV_CONFIG_REG){
        // Error
    }
}

static void PowerDownCO2Sensor(void) {
    PORTCbits.RC5 = 0;
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
    AdcConvert(IRCA1_OUT_AN_CH, &irca1Data.out);
    AdcConvert(IRCA1_TMP_AN_CH, &irca1Data.tmp);
    AdcConvert(IRCA1_COMMON_AN_CH, &irca1Data.com);
    AdcClose();
}

static void DisableCO2(void) {
    PowerDownCO2Sensor();
    Irca1DisableLamp();
}

static void EnableSHT11(void) {
    TRISAbits.TRISA5 = 0;
    PORTAbits.RA5 = 1;
}

static void MeasureSHT11(void) {
    Sht11_measure(&shtData);
}

static void DisableSHT11(void) {
    PORTAbits.RA5 = 0;
}
