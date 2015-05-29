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

#ifndef digi_serial_h
#define digi_serial_h

#include "../../bsp.h"

void XBeeSerialInit(uint8_t baudrate);

void XBeeSerialSend(uint8_t value);

void XBeeSerialSendArray(uint8_t* values, uint8_t size);

uint8_t XBeeSerialRead(void);

BOOL XBeeSerialAvailable(void);

void XBeeSerialClose(void);

BOOL XBeeSerialCheckInterrupt(void);

void XBeeSerialAckInterrupt(void);

#endif