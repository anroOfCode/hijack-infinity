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

// Hardware Pin Mappings for boards:
///////////////////////////////////////
// HIJACK REVD BOARD WITH MSP430F1611:
//
// Digital In 1: J4 (left), P1
// Digital In 2: J4 (left), P2
// Digital Out 1: J4 (left), P3
// Digital Out 2: J4 (left), P4
// Digital Out 3: J4 (left), P5
// Digital Out 4: J4 (left), P6
// Analog In 1: J2 (right), P5
// Analog In 2: J2 (right), P6
// 
//////////////////////////////////////

#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include "config.h"

#ifdef MSP430FR5969

#define LED_0 BIT0
#define LED_OUT P1OUT
#define LED_DIR P1DIR

#define LED_PORT 1
#define LED_PIN 0

#define MIC_PORT 3
#define MIC_PIN 6

#define LEFT_PORT 1
#define LEFT_PIN 2

#define VREF_PORT 3
#define VREF_PIN 1

#define OUT1_PORT 2
#define OUT1_PIN 4

#define OUT2_PORT 2
#define OUT2_PIN 3

#define OUT3_PORT 1
#define OUT3_PIN 7

#define OUT4_PORT 1
#define OUT4_PIN 6

#define AIN1_PORT 1
#define AIN1_PIN 4

#define AIN2_PORT 1
#define AIN2_PIN 5

#define TEMP_PORT 4
#define TEMP_PIN 2

#define DIN1_PORT 3
#define DIN1_PIN 5

#define DIN2_PORT 3
#define DIN2_PIN 4

#define AIN1_ACH 4
#define AIN2_ACH 5
#define TEMP_ACH 10
#define VREF_ACH 13

#endif

#ifdef MSP430F1611

#define LED_PORT 1
#define LED_PIN 2

#define MIC_PORT 4
#define MIC_PIN 0

#define LEFT_PORT 2
#define LEFT_PIN 3

#define VREF_PORT 6
#define VREF_PIN 1

#define OUT1_PORT 5
#define OUT1_PIN 0

#define OUT2_PORT 5
#define OUT2_PIN 1

#define OUT3_PORT 5
#define OUT3_PIN 2

#define OUT4_PORT 5
#define OUT4_PIN 3

#define AIN1_PORT 6
#define AIN1_PIN 4

#define AIN2_PORT 6
#define AIN2_PIN 5

#define TEMP_PORT 6
#define TEMP_PIN 6

#define DIN1_PORT 4
#define DIN1_PIN 5

#define DIN2_PORT 4
#define DIN2_PIN 6

#define AIN1_ACH 4
#define AIN2_ACH 5
#define TEMP_ACH 6
#define VREF_ACH 1

#endif

#endif