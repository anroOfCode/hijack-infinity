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

#include "adc.h"

#if defined(MSP430FR5969)

void adc_runConversion () { 
	ADC12CTL0 |= ADC12ENC + ADC12SC;
	while (ADC12CTL1 & ADC12BUSY); 
}

void adc_init2(void) {
	// Enable multiple conversions.
	// 16ADCclks, MSC, ADC ON
	ADC12CTL0 = ADC12SHT0_2 + ADC12MSC + ADC12ON;

	// sampling timer, s/w trig.,single sequence
	ADC12CTL1 = ADC12SHP + ADC12CONSEQ_1;   
	ADC12CTL2 = ADC12RES__12BIT;

	// 3.1
	P3SEL0 |= (1<<1);
	P3SEL1 |= (1<<1);

	// 1.4
	P1SEL0 |= (1<<4);
	P1SEL1 |= (1<<4);

	// 1.5
	P1SEL0 |= (1<<5);
	P1SEL1 |= (1<<5);

	// 4.2
	P4SEL0 |= (1<<2);
	P4SEL1 |= (1<<2);	

	// Configure the memory banks
	ADC12MCTL0 = ADC12INCH_4 + ADC12VRSEL1;
	ADC12MCTL1 = ADC12INCH_5 + ADC12VRSEL1;
	ADC12MCTL2 = ADC12INCH_10 + ADC12VRSEL1;
	ADC12MCTL3 = ADC12INCH_13 + ADC12VRSEL1 + ADC12EOS;	

	// Clear the ADC results - for debugging purposes.
	ADC12MEM0 = 0;
	ADC12MEM1 = 0;
	ADC12MEM2 = 0;
	ADC12MEM3 = 0;
}

uint16_t adc_getResult (uint8_t conv_num) {
	return *((&ADC12MEM0) + conv_num * 2);
}

#endif