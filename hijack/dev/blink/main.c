/*
 * main.c
 *  This code cycles periodically through 4 different cache hit ratios
 *  Created on: Feb 3, 2012
 *      Author: a0271502
 */
#include "msp430.h"
#include <inttypes.h>


#define LED_0 BIT0
#define LED_1 BIT1
#define LED_OUT P1OUT
#define LED_DIR P1DIR
//#define BUTTON BIT3

unsigned int blink = 0;

void board_init(void);

uint8_t count = 0;
uint16_t timer_counter = 0;

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

	board_init();
//	set_1MHz();

	LED_DIR |= LED_0; // Set P1.0 and P1.6 to output direction
	LED_OUT &= LED_0; // Set the LEDs off
//	P1IE |= BUTTON;


	TA0CCTL0 = CCIE;
	TA0CCR0  = 40000;
	TA0CTL   = TASSEL__SMCLK + MC__UP + ID__8;

	__enable_interrupt();

//	printf("hey there\n");

	LPM3;
	//while(1);

	return 0;

}


// Timer A0 interrupt service routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
	count++;
	LED_OUT ^= LED_0; // Clear the LEDs so they start in OFF state

	*((uint16_t*) 0x1974) = 0xF000;

	if (count == 0) {
		timer_counter += 20000;
		TA0CCR0 = timer_counter;
	}

}


void board_init(void)

{
	  P1DIR = 0;
	  P1OUT = 0;
	  P1REN = 0xFF;

	  P2DIR = 0;
	  P2OUT = 0;
	  P2REN = 0xFF;

	  P3DIR = 0;
	  P3OUT = 0;
	  P3REN = 0xFF;

	  P4DIR = 0;
	  P4OUT = 0;
	  P4REN = 0xFF;

	  PJDIR = 0;
	  PJOUT = 0;
	  PJREN = 0xFF;

	  CSCTL0_H = 0xA5;
	  CSCTL1 = DCOFSEL_0;                   // Set DCO= 1MHz
	  CSCTL1 &= ~DCORSEL;
	  CSCTL2 = SELA__VLOCLK + SELS__DCOCLK + SELM__DCOCLK;
	  CSCTL3 = DIVA__1 + DIVS__1 + DIVM__1; // set all dividers
	  CSCTL4 = LFXTOFF | HFXTOFF;
	  CSCTL5 &= ~(LFXTOFFG | HFXTOFFG);

}




