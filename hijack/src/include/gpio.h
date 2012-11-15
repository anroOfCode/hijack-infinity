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

#ifndef __GPIO_H__
#define __GPIO_H__

#include "config.h"

#if defined(MSP430FR5969) || defined(MSP430F1611)

#include <msp430.h>
#include <inttypes.h>

typedef enum gpio_dir {
	GPIO_IN,
	GPIO_OUT,
} gpio_dir_e;

void gpio_init (uint8_t port, uint8_t pin, gpio_dir_e dir);
void gpio_set_clear (uint8_t port, uint8_t pin, uint8_t set);
void gpio_set (uint8_t port, uint8_t pin);
void gpio_clear (uint8_t port, uint8_t pin);
void gpio_toggle(uint8_t port, uint8_t pin);
uint8_t gpio_read(uint8_t port, uint8_t pin);

#endif

#endif

