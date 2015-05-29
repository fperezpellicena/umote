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

#include "digi_api.h"
#include "digi_serial.h"

static uint8_t EUSART_9600 = 207;

static void XBeeInterruptInit(void);

void XBeeInit(void) {
    XBeeSerialInit(EUSART_9600);
    XBeeInterruptInit();
    XBeeJoin();
}

static void XBeeInterruptInit(void) {
    // INT0 rising edge mode
    INTCON2bits.INTEDG0 = 1;
    // Disable pullup
    INTCON2bits.RBPU = 0;
    // Enable INT0
    INTCONbits.INT0IE = 1;
    // Clear INT0 flag
    INTCONbits.INT0IF = 0;
    // Configure RB0 as input
    TRISBbits.TRISB0 = 1;
}