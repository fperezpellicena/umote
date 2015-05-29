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

static XBeePacket joinPacket;

static void XBeeCommissioning(uint8_t timesButtonPress);
static void XBeeSetSleepMode(uint8_t mode);

/*..........................................................................*/

/* Join xbee */
void XBeeJoin(void) {
    XBeeReset();
    Delay10KTCYx(100);
    // Simulate one press commissioning button
    XBeeCommissioning(COMMISSIONING_ONE_PRESS);
    Delay10KTCYx(100);
    // Set sleep mode 8
    XBeeSetSleepMode(DEEP_SLEEP_MODE);
}

/*..........................................................................*/

/* Reset xbee */
void XBeeReset(void) {
    static uint8_t command[3] = {'F', 'R', '\0'};
    XBeeCreatePacket(&joinPacket);
    XBeeCreateATCommandPacket(&joinPacket, 0x01, command);
    XBeeSendPacket(&joinPacket);
}

static void XBeeCommissioning(uint8_t timesButtonPress) {
    static uint8_t command[3] = {'C', 'B', '\0'};
    uint8_t params[2];
    params[0] = timesButtonPress;
    params[1] = '\0';
    XBeeCreatePacket(&joinPacket);
    XBeeCreateCompleteATCommandPacket(&joinPacket, 0x01, command, params, 0x01);
    XBeeSendPacket(&joinPacket);
}

static void XBeeSetSleepMode(uint8_t mode) {
    static uint8_t command[3] = {'S', 'M', '\0'};
    uint8_t params[2];
    params[0] = mode;
    params[1] = '\0';
    XBeeCreatePacket(&joinPacket);
    XBeeCreateCompleteATCommandPacket(&joinPacket, 0x01, command, params, 0x01);
    XBeeSendPacket(&joinPacket);
}
