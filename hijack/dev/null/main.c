
#include "msp430.h"

void board_init(void);


int main(void) {

	WDTCTL = WDTPW + WDTHOLD;

	board_init();

	__bis_SR_register(CPUOFF);

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




