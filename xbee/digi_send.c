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

/* Send XBee packet */
BOOL XBeeSendPacket(XBeePacket * const packet) {
    uint8_t* p = (uint8_t*) packet;
    XBeeSerialSend(START_DELIMITER);
    // send the most significant bit
    XBeeSerialSend((packet->length >> 8) & 0xFF);
    // then the LSB
    XBeeSerialSend(packet->length & 0xFF);
    // just in case it hasn't been initialized.
    packet->checksum = 0;
    //Generalizando para cualquier paquete
    while (packet->length--) {
        XBeeSerialSend(*p);
        packet->checksum += *p++;
    }
    XBeeSerialSend((0xFF - packet->checksum));
    // Bugfix: Flush buffer
    XBeeSerialSend(NULL);
    return TRUE;
}