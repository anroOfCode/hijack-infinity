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

#include "reference.h"

#if defined(MSP430FR5969)

void reference_enable () {
	while (REFCTL0 & REFGENBUSY);
	REFCTL0 |= REFON;
}

void reference_set (reference_voltage_e rv) {
	uint8_t ref_voltage = 0;

	while (REFCTL0 & REFGENBUSY);

	switch (rv) {
	  case REFVOLT_12: ref_voltage = REFVSEL_0; break;
	  case REFVOLT_20: ref_voltage = REFVSEL_1; break;
	  case REFVOLT_25: ref_voltage = REFVSEL_2; break;
	}

	REFCTL0 = 0;
	REFCTL0 = (REFCTL0 & ~REFVSEL_3) | ref_voltage;

}

#endif