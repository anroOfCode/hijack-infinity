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

#ifndef __ADC_H__
#define __ADC_H__

#include "config.h"

#if defined(MSP430FR5969)

#include "msp430.h"
#include <inttypes.h>
#include <stddef.h>
#include "utility.h"

// Cause a single conversion or a sequence to start.
void adc_runConversion ();

void adc_init2(void);

// Retrieve the result of a conversion. Waits for the busy bit to be cleared
//  before reading the value.
uint16_t adc_getResult (uint8_t conv_num);

#endif

#endif