#ifndef IRCA_H
#define	IRCA_H

#include <xc.h>

#define IRCA1_OUT_AN_CH         10
#define IRCA1_OUT_CNF           ANCON1bits.PCFG10   /* AN10 config*/
#define IRCA1_OUT_DDR           TRISBbits.TRISB1    /* LMP91051 output */

// FIXME Use when available in hardware
#define IRCA1_COMMON_AN_CH      8
#define IRCA1_COMMON_CNF        ANCON1bits.PCFG8    /* AN8 xonfig */
#define IRCA1_COMMON_DDR        TRISBbits.TRISB2    /* IRCA common */

#define IRCA1_TMP_AN_CH         11
#define IRCA1_TMP_CNF           ANCON1bits.PCFG11    /* AN11 config */
#define IRCA1_TMP_DDR           TRISCbits.TRISC2    /* IRCA temperature */

/* PWM OUTPUT CONFIGURATION FOR VPULSE */
#define IRCA1_CLK_DDR    TRISCbits.TRISC0    /* Vpulse */
#define IRCA1_RPIN       RPOR11              /* Remap-pin register */
#define IRCA1_TIMER      14                  /* Remap-pin register */
#define IRCA1_REG_PERIOD PR2                 /* Vpulse period */
#define IRCA1_DUTY       CCPR1L              /* Vpulse period */
#define IRCA1_TMR        T2CON               /* Timer counter */
#define IRCA1_CCP        CCP1CON             /* Timer counter */
#define IRCA1_PERIOD     255                 /* Period */
#define IRCA1_PRESCALE   0b00000010          /* Prescaler */
#define IRCA1_CCP_PWM    0b00001100          /* PWM mode */

/*...........................................................................*/
/* Class IrcA1Data sensor data */
typedef struct IrcA1Data IrcA1Data;

struct IrcA1Data {
    uint16_t out;
    uint16_t tmp;
    uint16_t com;
};

void Irca1Init(void);

void Irca1PowerUp(void);

void Irca1DisableLamp(void);

void Irca1EnableLamp(void);

#endif	/* IRCA_H */

