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

/*..........................................................................*/

void XBeeCreateTransmitRequestPacket(XBeePacket* packet, uint8_t frameId,
        uint8_t* destinationAddress, uint8_t radius, uint8_t options,
        uint8_t* payload, uint8_t dataLength) {
    uint8_t* p = packet->frame.transmitRequest.destinationAddress;
    uint8_t i = MAC_ADDRESS_BYTES_LENGTH;
    while (i--) {
        *p++ = *destinationAddress++;
    }
    p = packet->frame.transmitRequest.payload;
    i = dataLength;
    while (i--) {
        *p++ = *payload++;
    }
    p = packet->frame.transmitRequest.reserved;
    *p++ = TRANSMIT_REQUEST_RESERVED_H;
    *p++ = TRANSMIT_REQUEST_RESERVED_L;
    packet->frame.transmitRequest.bcastRadius = radius;
    packet->frame.transmitRequest.options = options;
    packet->apiId = TRANSMIT_REQUEST;
    packet->frame.transmitRequest.frameId = frameId;
    packet->length = TRANSMIT_REQUEST_COMMAND_MINLENGTH + dataLength;
}