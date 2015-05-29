#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void AdcInit(void);

void AdcConvert(uint8_t channel, uint16_t* co2);

#endif
