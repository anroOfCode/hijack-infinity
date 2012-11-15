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

#include <msp430.h>

void timer_init (void) {

}

void timer_start (void) {

}

void timer_setCaptureCallback (timer_captureCallback* cb) {

}

void timer_setPeriodicCallback (timer_periodicCallback* cb) {

}

void timer_stop (void) {

}

uint8_t timer_readCaptureLine (void) {
	return 0;
}

#endif
