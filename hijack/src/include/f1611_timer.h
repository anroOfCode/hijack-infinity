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

#ifndef __F1611_TIMER_H__
#define __F1611_TIMER_H__

#include "config.h"

#if defined(MSP430F1611)
 
#include <inttypes.h>

typedef void timer_captureCallback(uint16_t);
typedef void timer_periodicCallback(void);

timer_captureCallback *timer_captureCbPtr;
timer_periodicCallback *timer_periodicCbPtr;

void timer_init (void);
void timer_start (void);
void timer_setCaptureCallback (timer_captureCallback* cb);
void timer_setPeriodicCallback (timer_periodicCallback* cb);
void timer_stop (void);
uint8_t timer_readCaptureLine (void);

#endif

#endif