/*
 *  This file is part of hijack-infinity.
 *
 *  hijack-infinity is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  hijack-infinity is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with hijack-infinity.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "framingEngine.h"

/////////////////////////////
// Region: Public functions
/////////////////////////////

void fe_init(void) {
	fe_state.rxState = fe_receiveState_start;
	fe_state.incomingBufferPos = 0;
	fe_state.outgoingBufferPos = 0;
	fe_state.outgoingBufferSize = 0;

	fe_state.incomingPktSize = 0;
	fe_state.outgoingPktSize = 0;

	fe_state.receiveSize = 0;
}

void fe_handleByteReceived(uint8_t val) {

	// If it's a start-byte abandon what we were
	// doing previously and reset state back to start.
	if (val == 0xDD &&
		fe_state.rxState != fe_receiveState_dataEscape) {
		fe_state.rxState = fe_receiveState_size;
		fe_state.incomingBufferPos = 0;
		return;
	}

	switch (fe_state.rxState) {
		// More than most likely it's data
		// or an escape character:
		case fe_receiveState_data:
			if (val == 0xCC) {
				fe_state.rxState = fe_receiveState_dataEscape;
				fe_state.receiveSize--;
				break;
			}
			// INTENTIONAL FALL THROUGH
		case fe_receiveState_dataEscape:
			fe_state.incomingBuffer[fe_state.incomingBufferPos++] = val;
			if (fe_state.incomingBufferPos == fe_state.receiveSize) {
				fe_checkPacket();
				fe_state.rxState = fe_receiveState_start;
			}
			break;

		// The byte after the start byte is the size:
		case fe_receiveState_size:
			if (val > FE_RECEIVEMAX) {
				fe_state.rxState = fe_receiveState_start;
				break;
			}
			fe_state.receiveSize = val;
			fe_state.rxState = fe_receiveState_data;
			break;
		default:
			break;
	}
}

void fe_handleByteSent(void) {
	// If we've sent the entire packet then lets call
	// the application layer and have it generate another
	// packet.
	if (fe_state.outgoingBufferPos == fe_state.outgoingBufferSize) {
		fe_state.packetSentCb();
		fe_buildTransmitBuffer();
		fe_state.outgoingBufferPos = 0;
	}

	fe_state.byteSender(fe_state.outgoingBuffer[fe_state.outgoingBufferPos++]);
}

void fe_writeTxBuffer(uint8_t * buf, uint8_t len) {
	uint8_t i;
	for (i = 0; i < len; i++) {
		fe_state.outgoingPktBuffer[i] = buf[i];
	}
	fe_state.outgoingPktSize = len;
}

void fe_startSending(void) {
	fe_buildTransmitBuffer();
	fe_handleByteSent();
}

//////////////////////////////////
// Region: Callback Subscriptions
//////////////////////////////////

void fe_registerPacketReceivedCb(fe_packetReceived * cb) {
	fe_state.packetReceivedCb = cb;
}

void fe_registerPacketSentCb(fe_callback * cb) {
	fe_state.packetSentCb = cb;
}

void fe_registerByteSender(fe_byteSender * sender) {
	fe_state.byteSender = sender;
}

////////////////////////////
// Region: Helper functions
////////////////////////////

void fe_checkPacket() {
	uint8_t sum = 0;
	uint8_t i = 0;
	uint8_t incomingPktPos = 0;

	// Compute the simple byte-addition checksum.
	for (i = 0; i < fe_state.incomingBufferPos - 1; i++) {
		sum += fe_state.incomingBuffer[i];
	}

	// Fill incomingPktBuffer with the data.
	if (sum == fe_state.incomingBuffer[fe_state.incomingBufferPos - 1]) {

	//	if ((LED_OUT & LED_0) == 0){
	//		LED_OUT ^= LED_0;
	//	} else {
	//		LED_OUT &= ~LED_0;
	//	}

		// Ignore the checksum on the end.
		for (i = 0; i < fe_state.incomingBufferPos - 1; i++) {
			fe_state.incomingPktBuffer[incomingPktPos++] =
				fe_state.incomingBuffer[i];
		}

		fe_state.incomingPktSize = incomingPktPos;

		fe_state.packetReceivedCb(fe_state.incomingPktBuffer,
			fe_state.incomingPktSize);
	}
}

void fe_buildTransmitBuffer(void) {
	// DEPENDS ON:
	// - outgoingPktBuffer being full
	// - outgoingPktSize being accurate

	// MODIFIES:
	// - outgoingBuffer
	// - outgoingBufferSize
	// - outgoingBufferPos

	uint8_t i;
	uint8_t sum = 0;

	fe_state.outgoingBufferPos = 0;
	fe_state.outgoingBuffer[fe_state.outgoingBufferPos++] = 0xDD;
	fe_state.outgoingBuffer[fe_state.outgoingBufferPos++] =
		fe_state.outgoingPktSize + 1;

	for (i = 0; i < fe_state.outgoingPktSize; i++) {

		// Properly escape invalid characters.
		if (fe_state.outgoingPktBuffer[i] == 0xDD ||
			fe_state.outgoingPktBuffer[i] == 0xCC) {
			fe_state.outgoingBuffer[fe_state.outgoingBufferPos++] = 0xCC;
			sum += 0xCC;
			fe_state.outgoingBuffer[1]++;
		}

		fe_state.outgoingBuffer[fe_state.outgoingBufferPos++] =
			fe_state.outgoingPktBuffer[i];

		sum += fe_state.outgoingPktBuffer[i];
	}

	fe_state.outgoingBuffer[fe_state.outgoingBufferPos++] = sum;
	fe_state.outgoingBufferSize = fe_state.outgoingBufferPos;
	fe_state.outgoingBufferPos = 0;
}
