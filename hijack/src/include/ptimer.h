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
 
#ifndef __PTIMER_H__
#define __PTIMER_H__

#include "config.h"

#if defined(MSP430FR5969)

#include <inttypes.h>

typedef void ptimer_callback(void);


void ptimer_init ();
void ptimer_start (uint16_t ms, ptimer_callback* cb);
void ptimer_stop ();

#endif

#endif