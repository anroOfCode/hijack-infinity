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

#ifndef __PAL_H__
#define __PAL_H__

#include "config.h"
#include <inttypes.h>
#include <stdio.h>

#ifdef MSP430FR5969

#include <msp430.h>

#include "ptimer.h"
#include "ctimer.h"
#include "comparator.h"

#include "gpio.h"

#include "utility.h"
#include "hardware.h"
#include "analog.h"

#endif

//////////////////////
// Public Interface
//////////////////////

enum pal_gpioEnum {
	pal_gpio_din1,
	pal_gpio_din2,
	pal_gpio_ain1,
	pal_gpio_ain2,
	pal_gpio_vref,
	pal_gpio_temp,
	pal_gpio_dout1,
	pal_gpio_dout2,
	pal_gpio_dout3,
	pal_gpio_dout4,
	pal_gpio_mic
};

typedef void pal_periodicTimerCb(void);
typedef void pal_captureTimerCb(uint16_t elapsedTime, uint8_t isHigh);

void pal_init(void);
void pal_startTimers(void);

void pal_registerPeriodicTimerCb(pal_periodicTimerCb * fn);
void pal_registerCaptureTimerCb(pal_captureTimerCb * fn);

uint8_t pal_readDigitalGpio(enum pal_gpioEnum pin);
int8_t pal_setDigitalGpio(enum pal_gpioEnum pin, uint8_t val);

void pal_sampleAnalogGpios(void);

uint16_t pal_readAnalogGpio(enum pal_gpioEnum pin);

void pal_loopDelay(void);

//////////////////////
// Private Interface
//////////////////////

#ifdef MSP430FR5969
// 1Mhz Clock with a /8 divider and 3000 pulses a second.
#define TIMER_TICKS (16000000 / 8 / 3000)
void pal_periodicTimerFn(void);
void pal_captureTimerFn(uint16_t elapsedTimer);

#endif


#endif