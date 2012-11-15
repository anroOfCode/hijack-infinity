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
 
#ifndef __CTIMER_H__
#define __CTIMER_H__

#include "config.h"

#if defined(MSP430FR5969)


#include <inttypes.h>

typedef void ctimer_callback(uint16_t);


void ctimer_init (void);
void ctimer_set_callback (ctimer_callback* cb);
void ctimer_clear (void);
void ctimer_stop (void);
uint8_t ctimer_readLine (void);

#endif

#endif

