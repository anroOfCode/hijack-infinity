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

 /* 
  * Platform Abstraction Layer
  * ---------------------------
  * This file contains all the platform-specific hooks for
  * the HiJack codebase. Add your microcontroller-specific
  * code here, and include it using a #define statement
  * in config.h.
  */ 

#ifndef __PAL_H__
#define __PAL_H__

#include "config.h"

#include <inttypes.h>
#include <stdio.h>

//////////////////////
// Public Interface
//////////////////////

// The example applicatoin supports
// some basic digital/analog IO ports
// that we map to platform-specific
// ports and pins.
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
	pal_gpio_mic,
	pal_gpio_led
};

////////////////////////////////////
// Platform Independent Code
// This consists of the callback function definitions
// and their methods to register with the PAL.
////////////////////////////////////

// These are the two main interfaces the PAL provides
// to the upper layers. A periodic timer that needs
// to fire at a frequency of 
typedef void pal_periodicTimerCb(void);
typedef void pal_captureTimerCb(uint16_t elapsedTime, uint8_t isHigh);

void pal_registerPeriodicTimerCb(pal_periodicTimerCb * fn);
void pal_registerCaptureTimerCb(pal_captureTimerCb * fn);

/////////////////////////////////////////////
// Platform Specific Code
// Everything below the fold needs to be
// implemented for your target microcontroller.
// See the description for desired behavior.
/////////////////////////////////////////////

// Initializes the microcontroller. Should do
// things such as getting the oscilator up
// and running and configuring the pins
// to be in the correct state.
void pal_init(void);

// Called after the main code has initialized
// the coding state machine and other stuff.
// Should actually start the periodic and 
// capture time firing.
void pal_startTimers(void);

// Reads a GPIO pin, returns High => 1, Low => 0
uint8_t pal_readDigitalGpio(enum pal_gpioEnum pin);

// Sets a GPIO pin, 1 => High, 0=> Low
int8_t pal_setDigitalGpio(enum pal_gpioEnum pin, uint8_t val);

// Causes the analog subsystem to sample all of the
// analog GPIO pins.
void pal_sampleAnalogGpios(void);

// Reads a stored ADC result and returns it as an
// unsigned 16-bit integer
uint16_t pal_readAnalogGpio(enum pal_gpioEnum pin);

// Delay in the main loop. Not super important,
// should be maybe 250ms or so. 
void pal_loopDelay(void);

//////////////////////
// Private Interface
//////////////////////

// This stuff is platform specific...

#if defined(MSP430FR5969) || defined(MSP430F1611)

// 1Mhz Clock with a /8 divider and 3000 pulses a second.
#define TIMER_TICKS (16000000 / 8 / 3000)
void pal_periodicTimerFn(void);
void pal_captureTimerFn(uint16_t elapsedTimer);

#endif


#endif