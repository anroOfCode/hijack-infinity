
#include "msp430fr5969.h"



#define LED_0 BIT0
#define LED_1 BIT1
#define LED_OUT P1OUT
#define LED_DIR P1DIR
#define BUTTON BIT3

//unsigned int blink = 0;

void  set_1MHz(void);
void  set_8MHz(void);
void  set_16MHz(void);
void fram_chr_75(void);
void fram_chr_66(void);
void fram_chr_50(void);
void fram_chr_100(void);
void board_init(void);


void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

	board_init();
	set_1MHz();

	LED_DIR |= LED_0; // Set P1.0 and P1.6 to output direction
	LED_OUT &= ~LED_0; // Set the LEDs off
	P1IE |= BUTTON;

//	TA0CCTL0 = CCIE;
//	TA0CCR0 = 40000;
//	TA0CTL = TASSEL__ACLK + MC__UP;

	__enable_interrupt();

	LPM3;

/*	for (;;)
	{

		if(blink > 0)
		{
			P1OUT ^= (LED_0 + LED_1); // Toggle P1.0 and P1.6 using exclusive-OR

			__delay_cycles(100000); // SW Delay of 10000 cycles at 1Mhz

		}
	}*/

}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
//	blink ^= 0x01;
	P1IFG &= ~BUTTON; // P1.3 IFG cleared
	LED_OUT ^= LED_0;

}


/*
// Timer A0 interrupt service routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
	blink ^= 0x01;
	LED_OUT &= ~(LED_0 + LED_1); // Clear the LEDs so they start in OFF state
}
*/

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





/*


void  set_1MHz(void);
void  set_8MHz(void);
void  set_16MHz(void);
void fram_chr_75(void);
void fram_chr_66(void);
void fram_chr_50(void);
void fram_chr_100(void);
void CHR_prep(unsigned char);
void boardInit(void);



void main (void)
{
  unsigned char mode = 0;

  WDTCTL = WDTPW + WDTHOLD;

  boardInit();
  set_1MHz();
 //  Setup timer to provide slow interrupts
  TA0CCTL0 = CCIE;
  TA0CCR0 = 40000;
  TA0CTL = TASSEL__ACLK + MC__UP;

  mode = 0;
  while(1)
  {
	__enable_interrupt();
	mode = mode +2;
	if (mode>8)
		{
		mode = 2;
		}
	switch(__even_in_range(mode,10))
	  {
	  	case 0:
	  	      break;
	  	case 2:
	  	      CHR_prep(0);
	  	      fram_chr_100();
	  	      break;
	  	case 4:
	  		  CHR_prep(0);
	  	      fram_chr_75();
	  	      break;
	  	case 6:
	  		  CHR_prep(0);
	  	      fram_chr_66();
	  	      break;
	  	case 8:
	  		  CHR_prep(0);
	  	      fram_chr_50();
	  	      break;
	  	default:
	  	      break;
	  }
  }
}




void boardInit(void)

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


// Timer A0 interrupt service routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
  // call CHR to exit cache hit ratio subroutine
  // on exit from ISR
	CHR_prep(1);


}
*/

