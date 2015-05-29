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

#include "digi_serial.h"

void XBeeSerialInit(uint8_t baudrate) {
    TRISCbits.TRISC7 = 1;
    TRISCbits.TRISC6 = 0;

    RCSTA1bits.CREN = 1;
    TXSTA1bits.BRGH = 1;

    TXSTA1bits.TXEN = 1;
    RCSTA1bits.SPEN = 1;

    SPBRG1 = baudrate;

    BAUDCON1bits.BRG16 = 1;
}

void XBeeSerialSend(uint8_t value) {
    while (!TXSTA1bits.TRMT);
    TXREG1 = value;
}

void XBeeSerialSendArray(uint8_t* values, uint8_t size) {
    uint8_t i = 0;
    while (i < size) {
        XBeeSerialSend(values[i++]);
    }
    XBeeSerialSend(NULL);
}

uint8_t XBeeSerialRead(void) {
    return RCREG1;
}

BOOL XBeeSerialAvailable(void) {
    if (PIR1bits.RC1IF) {
        return TRUE; // Data is available, return TRUE
    }
    return FALSE;
}

void XBeeSerialClose(void) {
    RCSTA1 &= 0b01001111; // Disable the receiver
    TXSTA1bits.TXEN = 0; // and transmitter
    PIE1 &= 0b11001111; // Disable both interrupts
}

/** Interrupt handler functions */
BOOL XBeeSerialCheckInterrupt(void) {
    return XBeeSerialAvailable();
}

void XBeeSerialAckInterrupt(void) {
    PIR1bits.RC1IF = 0;
}