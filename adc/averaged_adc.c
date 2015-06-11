#include "averaged_adc.h"
#include <adc.h>

#define AVERAGE_FACTOR  16
#define DIV_AVERAGE     4

static void AdcCalibrate(void);
static uint16_t AdcReadValue(void);


/**
 * Configura el ADC
 */
void AdcInit() {
    ADCON0bits.VCFG = 0;
    ADCON1bits.ADFM = 1; /* Right adjust */
    ADCON1bits.ACQT = 0; /* Automatic acquisition time */
    ADCON1bits.ADCS = 0; /* Clk Fosc/2 */
    ANCON1bits.VBGEN = 0;
    ANCON1bits.VBG2EN = 0;
    ADCON0bits.ADON = 0; /* Start off powered */
}

/**
 * Devuelve una muestra promedio de AVERAGE_FACTOR muestras.
 * Inicia el proceso tomando una muestra dummy para compensar el efecto del ruido
 * de las entradas.
 * A continuación, promedia tantas muestras como indica el factor de conversión.
 */
void AdcConvert(uint8_t channel, uint16_t* result) {
    uint32_t tmp = 0;
    uint8_t i = AVERAGE_FACTOR;
    ADCON0bits.CHS = channel;
    ADCON0bits.ADON = 1;    // Power on
    AdcCalibrate();
    while (i--) {
        ADCON0bits.GO = 1;
        tmp += AdcReadValue();
    }
    *result = (uint16_t)tmp >> DIV_AVERAGE;
    ADCON0bits.ADON = 0;    // Power off
}

/**
 * Realiza una conversión dummy para compensar el ruido de las entradas.
 */
static void AdcCalibrate(void) {
    ADCON1bits.ADCAL = 1;
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO);
    ADCON1bits.ADCAL = 0;
}

/**
 * Lee el valor del conversor.
 */
static uint16_t AdcReadValue(void) {
    while (ADCON0bits.GO);
    return (((uint16_t) ADRESH) << 8) | (ADRESL);
}
