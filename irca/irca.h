#ifndef IRCA_H
#define	IRCA_H

#include "../bsp.h"

#define IRCA1_OUT_AN_CH         10
#define IRCA1_OUT_CNF           ANCON1bits.PCFG10   /* AN10 config*/
#define IRCA1_OUT_DDR           TRISBbits.TRISB1    /* LMP91051 output */

#define IRCA1_TMP_AN_CH         11
#define IRCA1_TMP_CNF           ANCON1bits.PCFG11    /* AN11 config */
#define IRCA1_TMP_DDR           TRISCbits.TRISC2    /* IRCA temperature */

/* PWM OUTPUT CONFIGURATION FOR VPULSE */
#define IRCA1_PULSE_DDR         TRISCbits.TRISC0    /* Vpulse */
#define IRCA1_PULSE             LATCbits.LATC0     /* Vpulse */

/* SPI interface */
#define LMP91051_CS_DDR         TRISBbits.TRISB3
#define LMP91051_CS             LATBbits.LATB3

#define LMP91051_SDI_DDR         TRISBbits.TRISB5
#define LMP91051_SDO_DDR         TRISBbits.TRISB4
#define LMP91051_SCK_DDR         TRISBbits.TRISB2
#define LMP91051_SWC_DDR         TRISAbits.TRISA2
#define LMP91051_SWC             LATAbits.LATA2



/*...........................................................................*/
/* Class IrcA1Data sensor data */
typedef struct IrcAData IrcAData;

struct IrcAData {
    uint16_t out;
    uint16_t tmp;
    uint16_t com;
};

void IrcaInit(void);

void IrcaMeasure(IrcAData* data);

void IrcaDisableLamp(void);

void IrcaEnableLamp(void);

#endif	/* IRCA_H */

