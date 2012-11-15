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

#include "ctimer.h"
\
#if defined(MSP430FR5969)
 
#include "msp430.h"

// Comparator timer
// A2

ctimer_callback* ctimer_callback_fn;


void ctimer_init (void) {
	TA2CTL   = TASSEL__SMCLK + MC__CONTINUOUS + ID__8;

	// CM   = capture on both edges
	// CCIS = select comparator e
	// CAP  = capture mode
	// CCIE = interrupt enable
	TA2CCTL1 = CM_3 + CCIS_1 + CAP + CCIE;
}

void ctimer_set_callback (ctimer_callback* cb) {
	ctimer_callback_fn = cb;
}

void ctimer_clear (void) {
	TA2R = 0x00;
}

void ctimer_stop (void) {
	TA2CTL &= MC__STOP;
}

uint8_t ctimer_readLine (void) {
	return !!(TA2CCTL1 & CCI);
}

#pragma vector = TIMER2_A1_VECTOR
__interrupt void Timer_A2 (void) {
	uint16_t capture_reg;
	capture_reg = TA2CCR1;

	if (capture_reg > 0x100) {
		ctimer_clear();
		ctimer_callback_fn(capture_reg);
	}

	TA2CCTL1 &= ~CCIFG;
}

#endif