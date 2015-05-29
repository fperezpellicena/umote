#include "irca.h"
#include "../adc/include/averaged_adc.h"
#include <spi.h>
#include <pps.h>

#define DEV_CONFIG_WRITE    (0x00)                   
#define DEV_CONFIG_READ     (0x80)
#define DEV_CONFIG_ACT      (0x30)                   // IN1 PGA1 OFF, PGA2 ON, CMN 1.15V, EXT_FILT
#define DEV_CONFIG_REF      (0xB0)                   // IN2 PGA1 OFF, PGA2 ON, CMN 1.15V, EXT_FILT
#define DAC_CONFIG_WRITE    (0x01)
#define DAC_CONFIG_OFFSET   (0x67)                   // DAC offset 850mV -> NDAC = 103d = 0x67h
#define SDIO_CONFIG_WRITE   (0x0F)
#define SDIO_ENABLE         (0xFEED)                 
#define SDIO_DISABLE        (0x0000)                 

static void Irca1InitIO(void);
static void Irca1InitPulseWave(void);
static void IrcaWriteConfig(uint8_t config);

static bool error;
static uint8_t config;

// Init sensor struct and hw associated
void IrcaInit(void) {
    AdcInit();
    Irca1InitIO();
    Irca1InitPulseWave();
}

void IrcaMeasure(IrcAData* data) {
    IrcaWriteConfig(DEV_CONFIG_ACT);
    AdcConvert(IRCA1_OUT_AN_CH, &data->out);
    IrcaWriteConfig(DEV_CONFIG_REF);
    AdcConvert(IRCA1_OUT_AN_CH, &data->com);
    AdcConvert(IRCA1_TMP_AN_CH, &data->tmp);
}

static void IrcaWriteConfig(uint8_t config) {
    LMP91051_CS = 0;
    OpenSPI2(SPI_FOSC_16, MODE_11, SMPEND);
    // Configure device registers
    WriteSPI2(DEV_CONFIG_WRITE);
    WriteSPI2(config);
    LMP91051_CS = 1;
}

void IrcaEnableLamp(void) {
    T1CONbits.TMR1ON = 1;
}

void IrcaDisableLamp(void) {
    T1CONbits.TMR1ON = 0;
}

static void Irca1InitIO(void) {
    LMP91051_SWC = 0;   // Switch fixed to write direction communication
    IRCA1_OUT_CNF = 0;  // LMP91051 as analog pin
    IRCA1_OUT_DDR = 1;  // LMP91051 output
    IRCA1_TMP_CNF = 0; // IRCA temperature analog pin
    IRCA1_TMP_DDR = 1; // IRCA temperature as input
    IRCA1_PULSE_DDR = 0; // IRCA lamp output
    IRCA1_PULSE = 0; // Initally low
    LMP91051_CS_DDR = 0; // Chip select output
    LMP91051_CS = 1; // Disabled
    // SPI config.
    LMP91051_SDI_DDR = 1;
    LMP91051_SDO_DDR = 0;
    LMP91051_SCK_DDR = 0;
    LMP91051_SWC_DDR = 0;
    PPSInput(PPS_SDI2, PPS_RP8);
    PPSOutput(PPS_RP5, PPS_SDO2);
    PPSOutput(PPS_RP7, PPS_SCK2);
}

static void Irca1InitPulseWave(void) {
    T1CONbits.TMR1CS = 1;   // Fosc
    T1CONbits.T1CKPS = 3;   // 1:8 prescaler
    T1CONbits.TMR1ON = 0;   // Disable
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 1;    // Enable interrupt
    IPR1bits.TMR1IP = 0;    // Low priority interrupt
}
