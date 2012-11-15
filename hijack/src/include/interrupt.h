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

#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "config.h"

#if defined(MSP430FR5969)

#include <inttypes.h>
 
typedef void interrupt_callback(void);

typedef enum interrupt_edge {
	LOW_TO_HIGH,
	HIGH_TO_LOW,
} interrupt_edge_e;

void interrupt_init ();

void interrupt_create (uint8_t port,
	                   uint8_t pin,
	                   interrupt_edge_e edge,
	                   interrupt_callback* cb);

void interrupt_remove (uint8_t port, uint8_t pin);

void interrupt_edge_direction (uint8_t port,
	                           uint8_t pin,
	                           interrupt_edge_e edge);

#endif

#endif