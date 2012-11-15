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

#ifndef __COMPARATOR_H__
#define __COMPARATOR_H__

#include "config.h"

#if defined(MSP430FR5969)

#include <inttypes.h>
 
typedef void comparator_callback(void);

// Puts the input pin in special input
// mode such that the special function is
// selected.
void comparator_enablePin (uint8_t port, uint8_t pin);

// Detaches the specified pin from the comparator
// so it can be measured.
void comparator_disablePin (uint8_t port, uint8_t pin);

uint8_t comparator_getChannel (uint8_t port, uint8_t pin);

void comparator_init ();
void comparator_on ();
void comparator_off ();

void comparator_setup (uint8_t compare_pin_neg, uint8_t compare_pin_pos);

void comparator_enableInterrupt (comparator_callback* cb);

#endif

#endif