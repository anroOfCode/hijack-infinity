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

 #include "interrupt.h"

#if defined(MSP430FR5969)

#include "msp430.h"

#include <stddef.h>

interrupt_callback* cb_fns[32];

void interrupt_init () {
	uint8_t i;

	for (i=0; i<32; i++) {
		cb_fns[i] = NULL;
	}
}

void interrupt_create (uint8_t port,
	                   uint8_t pin,
	                   interrupt_edge_e edge,
	                   interrupt_callback* cb) {

	uint8_t bit = 1 << pin;
	uint8_t index;

	switch (port) {
	  case 1:
		P1DIR  &= ~bit;
		if (edge == LOW_TO_HIGH)      P1IES &= ~bit;
		else if (edge == HIGH_TO_LOW) P1IES |= bit;
		P1IE   |= bit;
		P1SEL0 &= ~bit;
		P1SEL1 &= ~bit;
		break;
	  case 2:
		P2DIR  &= ~bit;
		if (edge == LOW_TO_HIGH)      P2IES &= ~bit;
		else if (edge == HIGH_TO_LOW) P2IES |= bit;
		P2IE   |= bit;
		P2SEL0 &= ~bit;
		P2SEL1 &= ~bit;
		break;
	  case 3:
		P3DIR  &= ~bit;
		if (edge == LOW_TO_HIGH)      P3IES &= ~bit;
		else if (edge == HIGH_TO_LOW) P3IES |= bit;
		P3IE   |= bit;
		P3SEL0 &= ~bit;
		P3SEL1 &= ~bit;
		break;
	  case 4:
		P4DIR  &= ~bit;
		if (edge == LOW_TO_HIGH)      P4IES &= ~bit;
		else if (edge == HIGH_TO_LOW) P4IES |= bit;
		P4IE   |= bit;
		P4SEL0 &= ~bit;
		P4SEL1 &= ~bit;
		break;
	}

	index = ((port - 1) * 7) + pin;

	cb_fns[index] = cb;
}

void interrupt_remove (uint8_t port, uint8_t pin) {

	uint8_t bit = 1 << pin;
	uint8_t index;

	switch (port) {
	  case 1:
//		P1DIR &= ~bit;
		P1IE  &= ~bit;
		break;
	  case 2:
//		P2DIR &= ~bit;
		P2IE  &= ~bit;
		break;
	  case 3:
//		P3DIR &= ~bit;
		P3IE  &= ~bit;
		break;
	  case 4:
//		P4DIR &= ~bit;
		P4IE  &= ~bit;
		break;
	}

	index = ((port - 1) * 7) + pin;

	cb_fns[index] = NULL;
}

void interrupt_edge_direction (uint8_t port,
	                           uint8_t pin,
	                           interrupt_edge_e edge) {

	uint8_t bit = 1 << pin;

	switch (port) {
	  case 1:
		if (edge == LOW_TO_HIGH)      P1IES &= ~bit;
		else if (edge == HIGH_TO_LOW) P1IES |= bit;
		break;
	  case 2:
		if (edge == LOW_TO_HIGH)      P2IES &= ~bit;
		else if (edge == HIGH_TO_LOW) P2IES |= bit;
		break;
	  case 3:
		if (edge == LOW_TO_HIGH)      P3IES &= ~bit;
		else if (edge == HIGH_TO_LOW) P3IES |= bit;
		break;
	  case 4:
		if (edge == LOW_TO_HIGH)      P4IES &= ~bit;
		else if (edge == HIGH_TO_LOW) P4IES |= bit;
		break;
	}
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {

	uint8_t pin_int;
	pin_int = (P1IV >> 1) - 1;
	if (cb_fns[pin_int] != NULL) {
		cb_fns[pin_int]();
	}
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) {

	uint8_t pin_int;
	pin_int = (P2IV >> 1) - 1;
	if (cb_fns[pin_int + 8] != NULL) {
		cb_fns[pin_int + 8]();
	}
}

#pragma vector=PORT3_VECTOR
__interrupt void Port_3(void) {

	uint8_t pin_int;
	pin_int = (P3IV >> 1) - 1;
	if (cb_fns[pin_int + 16] != NULL) {
		cb_fns[pin_int + 16]();
	}
}

#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void) {

	uint8_t pin_int;
	pin_int = (P4IV >> 1) - 1;
	if (cb_fns[pin_int] != NULL) {
		cb_fns[pin_int + 24]();
	}
}

#endif