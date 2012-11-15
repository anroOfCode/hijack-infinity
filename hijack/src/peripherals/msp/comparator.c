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

#include "comparator.h"

#if defined(MSP430FR5969)

#include "hardware.h"
#include "msp430.h"

comparator_callback* comparator_callback_fn;

void comparator_enablePin (uint8_t port, uint8_t pin) {
	if (comparator_getChannel(port, pin) == 0xFF) {
		return;
	}

	switch (port) {
	  case 1:
		P1SEL0 |= 1 << pin;
		P1SEL1 |= 1 << pin;
		break;
	  case 2:
		P2SEL0 |= 1 << pin;
		P2SEL1 |= 1 << pin;
		break;
	  case 3:
		P3SEL0 |= 1 << pin;
		P3SEL1 |= 1 << pin;
		break;
	}
}

void comparator_disablePin (uint8_t port, uint8_t pin) {
	if (comparator_getChannel(port, pin) == 0xFF) {
		return;
	}

	switch (port) {
	  case 1:
		P1SEL0 &= ~(1 << pin);
		P1SEL1 &= ~(1 << pin);
		break;
	  case 2:
		P2SEL0 &= ~(1 << pin);
		P2SEL1 &= ~(1 << pin);
		break;
	  case 3:
		P3SEL0 &= ~(1 << pin);
		P3SEL1 &= ~(1 << pin);
		break;
	}
}

uint8_t comparator_getChannel (uint8_t port, uint8_t pin) {

	uint8_t channel = 0xFF;

	switch (port) {
	  case 1:
		if (pin < 6) {
			channel = pin;
		}
		break;
	  case 2:
		if (pin == 3 || pin == 4) {
			channel = pin + 7;
		}
		break;
	  case 3:
		if (pin < 4) {
			channel = pin + 12;
		}
		break;
	}

	return channel;
}

void comparator_init () {
	// set the comparator in normal power mode
//	CBCTL1 = (CBCTL1 & ~CBPWRMD_0) | CBPWRMD_1;

	// set the comparator in high speed mode
	CBCTL1 &= ~CBPWRMD_0;


}

void comparator_on () {
	CBCTL1 |= CBON;
}

void comparator_off () {
	CBCTL1 &= ~CBON;
}

void comparator_setup (uint8_t compare_pin_neg, uint8_t compare_pin_pos) {
	// enable external inputs for both inputs of the comparator
	CBCTL0 = CBIMEN + (compare_pin_neg << 8) + CBIPEN + compare_pin_pos;

	// disable the input buffers for the pins being used
	CBCTL3 = (1 << compare_pin_neg) + (1 << compare_pin_pos);

}

void comparator_enableInterrupt (comparator_callback* cb) {
	// clear any interrupts
	CBINT &= ~(CBIFG + CBIIFG);
	CBINT |= CBIE;

	comparator_callback_fn = cb;
}

#pragma vector=COMP_B_VECTOR
__interrupt void Comp_B_ISR (void) {
	CBCTL1 ^= CBIES;              // Toggles interrupt edge
	CBINT  &= ~CBIFG;              // Clear Interrupt flag

	comparator_callback_fn();
}

#endif