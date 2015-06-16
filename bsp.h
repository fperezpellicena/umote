#ifndef BSP_H
#define	BSP_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#define NO_MOBILE   0
#define MOBILE      1

#define SHT_ID      'S'
#define IRCA_ID     'I'
#define CO2D1_ID    'C'
#define BMP085      'B'

void BspInit(void);

#endif	/* BSP_H */

