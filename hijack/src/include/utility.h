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

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "config.h"

#if defined(MSP430FR5969) || defined(MSP430F1611)

#include <msp430.h>
#include <inttypes.h>
#include <stdio.h>

// Disables the watchdog timer
void util_disableWatchdog(void);

void util_boardInit(void);

void util_enableInterrupt(void);

void util_disableInterrupt(void);

void util_delayMs(uint16_t ms);

// DO NOT USE THIS FUNCTION
// unless you want to delay less than a MS
void util_delayCycles (uint16_t cycles);

#endif

#endif