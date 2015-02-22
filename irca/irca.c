#include "../bsp.h"
#include "irca.h"
#include "../register.h"

static void Irca1InitIO(void);
static void Irca1InitPWM(void);

static void Irca1InitIO(void) {
    IRCA1_OUT_CNF = 0;          /* LMP91051 as analog pin */
    IRCA1_OUT_DDR = 1;          /* LMP91051 output */
    IRCA1_COMMON_CNF = 0;
    IRCA1_COMMON_DDR = 1;
    IRCA1_TMP_CNF = 0;          /* IRCA temperature analog pin */
    IRCA1_TMP_DDR = 1;      /* IRCA temperature as input */
    IRCA1_CLK_DDR = 0;      /* IRCA lamp output */
}

static void Irca1InitPWM(void) {
    IRCA1_REG_PERIOD = IRCA1_PERIOD; /* PWM period */
    IRCA1_DUTY = IRCA1_PERIOD / 2;   /* PWM duty */
    IRCA1_TMR |= IRCA1_PRESCALE;    /* Prescale 16 */
    IRCA1_CCP |= IRCA1_CCP_PWM;     /* PWM mode*/
}

/* Init sensor struct and hw associated */
void Irca1Init(void) {
    Irca1InitIO();
    RegisterRemap((uint8_t*)&IRCA1_RPIN, IRCA1_TIMER);
    Irca1InitPWM();
}

void Irca1EnableLamp(void) {
    T2CONbits.TMR2ON = 1;
}

void Irca1DisableLamp(void) {
    T2CONbits.TMR2ON = 0;
}
