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

#ifdef __MSP430F1611__
	#define MSP430F1611
#endif

#ifdef __MSP430FR5969__
	#define MSP430FR5969
#endif

#if defined(MSP430FR5969)

#define IDLE_CYCLES 4

// Used by the coding state machine to identify
// long and short pulses.
#define THRESHOLD 325
#define DELTAT 775

#endif

#if defined(MSP430F1611)

#define IDLE_CYCLES 4
// Used by the coding state machine to identify
// long and short pulses.
#define THRESHOLD 4
#define DELTAT 13

#endif
