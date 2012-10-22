;*****************************************************************************
;
;  Project:    MSP430FR5969
;
;  (C) Texas Instruments, Deutschland GmbH
;      MSP430 Design
;      All Rights Reserved.
;
;
;*****************************************************************************
 .cdecls C,LIST,  "msp430fr5969.h"
            .global set_16MHz
            .global set_8MHz
            .global set_4MHz
            .global set_2MHz
            .global set_1MHz
            .global fram_chr_50
            .global fram_chr_66
            .global fram_chr_75
            .global fram_chr_100
            .global CHR_prep
;-----------------------------------------------------------------------------
            .text                                    ; Program reset
;-----------------------------------------------------------------------------
; set 16 MHZ
;-------------------------------------------------------------------------------
set_16MHz
            mov.w   #CSKEY,&CSCTL0					 ; enable CS
            mov.w   #DCOFSEL_4+DCORSEL,&CSCTL1		 ; set dco to 16MHz
            mov.w   #SELA__VLOCLK+SELM__DCOCLK+SELS__DCOCLK,&CSCTL2
            mov.w   #DIVS__8,&CSCTL3	             ; DIVS=8, DIVM=0
            clr.b   &(CSCTL0+1)			             ; disable CS
            reta

;-------------------------------------------------------------------------------
; set 8 MHZ
;-------------------------------------------------------------------------------
set_8MHz
            mov.w   #CSKEY,&CSCTL0	                 ; enable CS
            mov.w   #DCOFSEL_6,&CSCTL1
            mov.w   #SELA__VLOCLK+SELM__DCOCLK+SELS__DCOCLK,&CSCTL2
            mov     #DIVS__4,&CSCTL3	             ; DIVS=4, DIVM=0
            clr.b   &(CSCTL0+1)	                     ; disable CS
            reta

;-------------------------------------------------------------------------------
; set 4 MHZ
;-------------------------------------------------------------------------------
set_4MHz
            mov.w   #CSKEY,&CSCTL0	                 ; enable CS
            mov.w   #DCOFSEL_3,&CSCTL1
            mov.w   #SELA__VLOCLK+SELM__DCOCLK+SELS__DCOCLK,&CSCTL2
            mov     #DIVS__4,&CSCTL3                 ; DIVS=4, DIVM=0
            clr.b   &(CSCTL0+1)                      ; disable CS
            reta

;-------------------------------------------------------------------------------
; set 2.667 MHZ
;-------------------------------------------------------------------------------
set_2MHz
            mov.w   #CSKEY,&CSCTL0	                 ; enable CS
            mov.w   #DCOFSEL_1,&CSCTL1
            mov.w   #SELA__VLOCLK+SELM__DCOCLK+SELS__DCOCLK,&CSCTL2
            mov     #DIVS__2,&CSCTL3                 ; DIVS=2, DIVM=0
            clr.b   &(CSCTL0+1)                      ; disable CS
            reta

;-------------------------------------------------------------------------------
; set 1 MHZ
;-------------------------------------------------------------------------------
set_1MHz
            mov.w   #CSKEY,&CSCTL0	                 ; enable CS
            mov.w   #DCOFSEL_0,&CSCTL1
            mov.w   #SELA__VLOCLK+SELM__DCOCLK+SELS__DCOCLK,&CSCTL2
            mov     #0,&CSCTL3                       ; DIVS=0, DIVM=0
            clr.b   &(CSCTL0+1)                      ; disable CS
            reta


;-------------------------------------------------------------------------------
;     Prepare registers for FRAM CHR test code
;-------------------------------------------------------------------------------
CHR_prep
             clr.w  R4
             mov.b  R12,R4
             clr.w  R12
             reta
;-------------------------------------------------------------------------------
;     FRAM access with cache on hit/miss rate 1/1 CHR = 50%
;-------------------------------------------------------------------------------
            .align 8   ; align on 64bit boundary (8x 8bit)
fram_chr_50
            nop
            nop
            nop
            nop

            nop
            nop
            nop
            nop

            nop
            nop
chr_50_0    bit.b   #0x01,R4 ; nop
            jne     chr_50_exit_loop ; nop
            jmp     chr_50_1
            nop

            nop
            nop
            nop
            nop

            nop
            nop
chr_50_1    jmp chr_50_2
			nop

			nop
			nop
			nop
			nop

			nop
			nop
chr_50_2    jmp chr_50_3
            nop

            nop
            nop
            nop
            nop

            nop
            nop
chr_50_3    jmp chr_50_4
			nop

			nop
			nop
			nop
			nop

			nop
			nop
chr_50_4    jmp chr_50_5
			nop

			nop
			nop
			nop
			nop

			nop
			nop
chr_50_5    jmp chr_50_6
			nop

			nop
			nop
			nop
			nop

			nop
			nop
chr_50_6    jmp chr_50_0
			nop
			nop

chr_50_exit_loop
			reta

;-------------------------------------------------------------------------------
;     FRAM access with cache on hit/miss rate 2/1 CHR = 66%
;-------------------------------------------------------------------------------

            .align 8                        ; align on 64bit boundary (8x 8bit)
fram_chr_66
            nop
chr_66_0    bit.b   #01h,R4
            jnz     chr_66_exit_loop
            jmp     chr_66_1

            nop
            nop
            nop
            nop

            nop
chr_66_1    nop
            jmp     chr_66_2
            nop

            nop
            nop
            nop
            nop

            nop
chr_66_2    nop
            jmp     chr_66_3
            nop

            nop
            nop
            nop
            nop

            nop
chr_66_3    nop
            jmp     chr_66_4
            nop

            nop
            nop
            nop
            nop

            nop
chr_66_4    nop
            jmp     chr_66_5
            nop

            nop
            nop
            nop
            nop

            nop
chr_66_5    nop
			jmp     chr_66_0
			nop

			nop

chr_66_exit_loop
			reta

;-------------------------------------------------------------------------------
;     FRAM access with cache on hit/miss rate 3/1 CHR = 75%
;-------------------------------------------------------------------------------
fram_chr_75

            .align 8       ; align on 64bit boundary (8x 8bit)

chr_75_0    bit.w   #01h,R4
            jnz     chr_75_exit_loop
            jmp     chr_75_1
            nop

            nop
            nop
            nop
            nop

chr_75_1    nop
            nop
            jmp     chr_75_2
            nop

            nop
            nop
            nop
            nop

chr_75_2    nop
            nop
            jmp     chr_75_3
            nop

            nop
            nop
            nop
            nop

chr_75_3    nop
            nop
            jmp     chr_75_4
            nop

            nop
            nop
            nop
            nop

chr_75_4    nop
            nop
            jmp     chr_75_0                                                 ; endless loop
            nop

            nop

chr_75_exit_loop
            reta

;------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
;     FRAM access with cache on hit/miss rate 1/0 CHR = 100%
;-------------------------------------------------------------------------------
            .align 8       ; align on 64bit boundary (8x 8bit)

fram_chr_100
            nop;  clr.w   R4
            nop
            nop
            nop

chr_100_0   bit.b   #0x01,R4
            jnz     chr_100_exit_loop
            jmp     chr_100_0
chr_100_exit_loop
			reta

;---------------------------------------------------------------------------
            .END




