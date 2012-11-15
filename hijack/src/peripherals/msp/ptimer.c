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

#include "ptimer.h"

#if defined(MSP430FR5969)
 
// Periodic timer
#include "msp430.h"

uint8_t          ptimer_inuse = 0;
ptimer_callback* ptimer_callback_fn;

void ptimer_init () {
	TA1CCTL0 = CCIE;
	TA1CTL   = TASSEL__SMCLK + MC__STOP + ID__8;
}

void ptimer_start (uint16_t timerTicks, ptimer_callback* cb) {
	if (ptimer_inuse) {
		return;
	}

	// reset timer count
	TA1R = 0x00;

	// set count to val
	TA1CCR0 = timerTicks;

	// save callback
	ptimer_callback_fn = cb;
	ptimer_inuse = 1;

	// start timer
	TA1CTL  |= MC__UP;
}

void ptimer_stop () {
	TA1CTL &= MC__STOP;
	ptimer_inuse = 0;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer_A1 (void) {
	ptimer_callback_fn();
}

#endif