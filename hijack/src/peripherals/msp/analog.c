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

#include "analog.h"

#if defined(MSP430FR5969)

void analog_init(void) {
	// Turn on the 2.0V Reference Generator
	reference_set(REFVOLT_20);
	reference_enable();	
	adc_init2();
}

void analog_sampleAll(void) {
	adc_runConversion();
}

uint16_t analog_readInput(enum analog_inputEnum input) {
	switch (input) {
		case analog_input_vcc:
			return adc_getResult(0);
		case analog_input_extTemp:
			return adc_getResult(1);
		case analog_input_in1:
			return adc_getResult(2);
		case analog_input_in2:
			return adc_getResult(3);
	}

	return -1;
}

#endif

#if defined(MSP430F1611)
void analog_init(void) {
;
}

void analog_sampleAll(void) {
}

uint16_t analog_readInput(enum analog_inputEnum input) {
	return 0;
}

#endif