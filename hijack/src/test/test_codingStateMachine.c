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

#include "codingStateMachine.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

uint8_t receivedByte = 0x00;

const uint16_t sigTime = 1000;

void storeReceivedByte(uint8_t out) {
	printf("New byte received from csm: %x\n", out);
	receivedByte = out;
}

void setTime(struct csm_timer_struct * in, bool isLong) {
	in->signal = in->signal == 0 ? 1 : 0;
	in->elapsedTime = isLong ? sigTime * 2 : sigTime;
}

// Sends START + 0b11001010 + Parity Bit to the Decoder
void decoderTest() {
	// 1 - high to low transistion
	// 0 - low to high transistion
	// The sequence looks something like this:
	// 0, 0, 1 (START), 1, 1, 0, 0, 1, 0, 1, 0, 0 (PARITY)
	//  L H L H H L H L H L L H L H H L L H H L L H L H L H
	// * S S S   L S S S S   L S S   L   L   L   L S S S S

	struct csm_timer_struct inData;
	inData.elapsedTime = sigTime;
	inData.signal = 1;

	// The duration from the comment above, encoded.
	uint32_t durationEncoding = 0b00001111001000010000;
	uint32_t numberBits = 20;

	while (numberBits > 0) {
		// Let's start receiving some edges.
		bool isLong = (durationEncoding & 0x01) == 1;
		durationEncoding = (durationEncoding >> 1);

		setTime(&inData, isLong);
		csm_receiveTiming(&inData);

		numberBits--;
	}
}

void encoderTest() {
	int i = 0;

	// Show the toggling of the idle line
	// here... 0101010101.....
	for(i = 0; i < 8; i++) {
		printf("Idle line togg: %d\n", csm_advanceTransmitState());
	}

	csm_sendByte(0xCA);

	for (i = 0; i < 20; i++) {
		printf("Data: %d\n", csm_advanceTransmitState());
	}
	
	for(i = 0; i < 8; i++) {
		printf("Idle line togg: %d\n", csm_advanceTransmitState());
	}
}

int main(int argc, char ** argv) {
	// Setup the manchester state machine
	csm_initialize();
	csm_registerReceiveByte(&storeReceivedByte);
	decoderTest();
	encoderTest();
	printf("Test done!\n");
}


