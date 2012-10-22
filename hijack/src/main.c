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

#include <inttypes.h>
#include <stdio.h>

#include "pal.h"
#include "codingStateMachine.h"
#include "framingEngine.h"

uint8_t outMessage[] = {
	0xFF, 		// Make the first 2 bits of this
				// equal to the state of two input pins.
	0x3F, 0xFF, // Analog In 1
	0x3F, 0xFF, // Analog In 2
	0x3F, 0xFF, // Analog In 3 (temperature)
	0x2F, 0xFF  // Analog In 4 (potentially a v_ref)
};

uint16_t temp_buf[100] = {0};
uint8_t buf_idx= 0;

void periodicTimerFn (void) {

	uint8_t ats;
	ats = csm_advanceTransmitState();

	if (ats) {
		pal_setDigitalGpio(pal_gpio_mic, 1);
	} else {
		pal_setDigitalGpio(pal_gpio_mic, 0);
	}

	csm_finishAdvanceTransmitState();
}

void captureTimerFn(uint16_t elapsedTime, uint8_t isHigh) {
	struct csm_timer_struct timingData;
	timingData.elapsedTime = elapsedTime;
	timingData.signal = !isHigh;
	csm_receiveTiming(&timingData);	
}

void packetReceivedCallback(uint8_t * buf, uint8_t len) {
	if (len == 1) {
		pal_setDigitalGpio(pal_gpio_dout1, ((buf[0] >> 0) & 0x01));
		pal_setDigitalGpio(pal_gpio_dout2, ((buf[0] >> 1) & 0x01));
		pal_setDigitalGpio(pal_gpio_dout3, ((buf[0] >> 2) & 0x01));
		pal_setDigitalGpio(pal_gpio_dout4, ((buf[0] >> 3) & 0x01));
	}
}

void packetSentCallback(void) {
	fe_writeTxBuffer(outMessage, 9);
}

void updateAnalogOutputBuffer(void) {
	//pal_sampleAnalogGpios();
	uint16_t temp_ext = 0;
	uint16_t vcc = 0;
	uint16_t sample1 = 0;
	uint16_t sample2 = 0;

	//vcc = pal_readAnalogGpio(pal_gpio_vref);
	//temp_ext = pal_readAnalogGpio(pal_gpio_temp);
	//sample1 = pal_readAnalogGpio(pal_gpio_ain1);
	//sample2 = pal_readAnalogGpio(pal_gpio_ain2);

	outMessage[1] = vcc & 0xFF;
	outMessage[2] = (vcc >> 8) & 0xFF;
	outMessage[3] = temp_ext & 0xFF;
	outMessage[4] = (temp_ext >> 8) & 0xFF;
	outMessage[5] = sample1 & 0xFF;
	outMessage[6] = (sample1 >> 8) & 0xFF;
	outMessage[7] = sample2 & 0xFF;
	outMessage[8] = (sample2 >> 8) & 0xFF;
}

void updateDigitalOutputBuffer(void) {
	outMessage[0] = (outMessage[0] & ~(1 << 0)) |
		((pal_readDigitalGpio(pal_gpio_din1) & 0x1) << 0);
	outMessage[0] = (outMessage[0] & ~(1 << 1)) |
		((pal_readDigitalGpio(pal_gpio_din2) & 0x1) << 1);
}

void initializeSystem(void) {
	pal_init();

	// Initialize the transition-edge-length
	// manchester decoder.
	csm_init();
	csm_registerReceiveByte(fe_handleByteReceived);
	csm_registerTransmitByte(fe_handleByteSent);

	// Initialize the framing engine to process
	// the raw byte stream.
	fe_init();
	fe_registerPacketReceivedCb(packetReceivedCallback);
	fe_registerPacketSentCb(packetSentCallback);
	fe_registerByteSender(csm_sendByte);

	pal_registerPeriodicTimerCb(periodicTimerFn);
	pal_registerCaptureTimerCb(captureTimerFn);

	// Start the interrupt-driven timers.
	pal_startTimers();
	
	// Start the transmit callback-driven
	// loop
	fe_writeTxBuffer(outMessage, 9);
	fe_startSending();
}

int main () {
	initializeSystem();
	// TODO: Add sleep commands!

	while(1) {
		updateDigitalOutputBuffer();
		updateAnalogOutputBuffer();

		pal_loopDelay();
	}
    return 0;
}
