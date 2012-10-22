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

#include "msp430.h"
#include <inttypes.h>
#include "sstimer.h"



uint8_t           sstimer_inuse = 0;
sstimer_callback* sstimer_callback_fn;


void sstimer_init () {
	TA0CCTL0 = CCIE;
	TA0CTL   = TASSEL__SMCLK + MC__STOP + ID__8;
}

void sstimer_set (uint16_t ms, sstimer_callback* cb) {

	if (sstimer_inuse) {
		return;
	}

	// convert ms to timer tics

	// reset timer count
	TA0R = 0x00;

	// set count to val
	TA0CCR0 = ms;

	// start timer
	TA0CTL  |= MC__UP;

	// save callback
	sstimer_callback_fn = cb;
	sstimer_inuse = 1;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void) {

	TA0CTL &= MC__STOP;

	sstimer_inuse = 0;

	sstimer_callback_fn();
}



