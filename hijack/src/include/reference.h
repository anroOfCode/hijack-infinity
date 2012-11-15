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

#ifndef __REFERENCE_H__
#define __REFERENCE_H__

#include "config.h"

#if defined(MSP430FR5969)

#include <inttypes.h>
#include "msp430.h"
 
typedef enum reference_voltage {
	REFVOLT_12,
	REFVOLT_20,
	REFVOLT_25,
} reference_voltage_e;

void reference_enable ();

void reference_set (reference_voltage_e rv);

#endif

#endif
