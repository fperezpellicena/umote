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

#ifndef payload_h
#define payload_h

#include "../../bsp.h"

#define MAX_PAYLOAD     100

/* Static List struct */
typedef struct Payload Payload;

struct Payload {
    uint8_t data[MAX_PAYLOAD];
    uint8_t size;
};

/* Init list */
void PayloadInit(Payload* payload);

/* Add one byte to the list */
void Payload_putByte(Payload* payload, const uint8_t element);

/* Add one word to the list */
void PayloadPutWord(Payload* payload, const uint16_t element);

/* Append */
void Payload_append(Payload* to, const Payload* from);

/* Put a formatted string in ROM */
void Payload_putString(Payload* payload, const uint8_t* string);

/* Put a formatted string in RAM */
void Payload_putRAMString(Payload* payload, const uint8_t* string);

#endif /* list_H*/
