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

#include "f1611_timer.h"

#ifdef MSP430F1611

#include "ptimer.h"
#include "ctimer.h"

#include "pal.h"

#include <msp430.h>

void timer_init (void) {
	//////////////////////////////////
	// Comparator

    // Comp. A Int. Ref. enable on CA1
    CACTL1 = CARSEL+CAREF_2;
    // Comp. A Int. Ref. Select 2 : 0.5*Vcc
    // Comp. A Connect External Signal to CA0
    // Comp. A Enable Output Filter
    // enable comparator CA0 on P2.3 (P40 on epic)
	CACTL2 = P2CA0+CAF;   
	// enable comparator
	CACTL1 |= CAON;

	///////////////////////////
	// TimerA - Capture Timer
	TACTL = TASSEL_1 + TACLR;
	TACCTL1 = CM_3 + CCIS_1 + CAP + CCIE;

	///////////////////////////
	// TimerB - Periodic Timer
	TBCTL = TBSSEL_1 + TBIE + TBCLR;
	TBCCTL0 = 0;
	TBCCR0 = DELTAT;
}

void timer_start (void) {
	TACTL |= MC_2;
	TBCTL |= MC_1;
}

void timer_setCaptureCallback (timer_captureCallback* cb) {
	timer_captureCbPtr = cb;
}

void timer_setPeriodicCallback (timer_periodicCallback* cb) {
	timer_periodicCbPtr = cb;
}

void timer_stop (void) {
	TACTL &= ~MC_2;
	TBCTL &= ~MC_1;
}

uint8_t timer_readCaptureLine (void) {
	return !(TACCTL1 & CCI);
}

#pragma vector = TIMERA1_VECTOR
__interrupt void Timer_A1 (void) {
	
	uint16_t captureReg = TAR;

	if (captureReg > 5) {
		TAR = 0;
		timer_captureCbPtr(captureReg);
	}

	TACCTL1 &= ~CCIFG;
}

#pragma vector = TIMERB1_VECTOR
__interrupt void Timer_B1 (void) {
	timer_periodicCbPtr();
	TBCTL &= ~TBIFG;
}


#endif
