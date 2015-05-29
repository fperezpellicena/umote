/**
 * This file is part of uMote.
 *
 *  uMote is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *  uMote is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with uMote.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef sht11_h
#define sht11_h

#include <stdint.h>

#define SHT_MEASURE_TEMP    0x03    		/* Measure temperature command */
#define SHT_MEASURE_HUMI    0x05    		/* Measure humidity command */
#define SHT_STAT_REG_R      0x07    		/* Read status register command */
#define SHT_STAT_REG_W      0x06    		/* Write status register command */
#define SHT_RESET           0x1E    		/* Reset */

#define SHT_LOW_POWER       0x01                /* 8bit RH / 12 bit TMP */

#define SHT_ACK             1			/* Send ACK */
#define SHT_NACK            0                   /* Not send ACK */

/* Sht data class */
typedef struct {

    union {
        uint16_t i;
    } temperature;

    union {
        uint16_t i;
    } humidity;
    uint8_t tempChk;
    uint8_t humiChk;
} ShtData;

/* Init pins and registers*/
void Sht11Init(void);

/* Measures temperature and humidity */
uint8_t Sht11Measure(ShtData* data);

#endif /* sht11_h */
