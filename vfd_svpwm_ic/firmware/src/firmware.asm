; ******************************************************************************
; *                A V R   S V P W M   S U B P R O C E S S O R                 *
; *                               firmware.asm                                 *
; *                          (C) Christoph Klie 2019                           *
; *                            TAB SIZE =  4 SPACES                            *
; ******************************************************************************


; ==============================================================================
;             D  E  V  I  C  E     D  E  F  I  N  I  T  I  O  N  S
; ==============================================================================
.INCLUDE "tn861Adef.inc"


; ==============================================================================
;          R  E  G  I  S  T  E  R     D  E  F  I  N  I  T  I  O  N  S
; ==============================================================================
.def TEMPL      = r16                   ; temp register low byte
.def TEMPH      = r17                   ; temp register high byte


; ==============================================================================
;         V  A  R  I  A  B  L  E     D  E  F  I  N  I  T  I  O  N  S
; ==============================================================================
.dseg


; ==============================================================================
;              I  N  T  E  R  R  U  P  T     V  E  C  T  O  R  S
; ==============================================================================
.cseg
.org 0x0000                             ; RESET program address
    rjmp    INIT
.org 0x0001                             ; INT0 program address
    reti
.org 0x0002                             ; PCINT program address
    rjmp    _INTF_SPI_SS_ISR
.org 0x0003                             ; TIMER1_COMPA program address
    reti
.org 0x0004                             ; TIMER1_COMPB program address
    reti
.org 0x0005                             ; TIMER1_OVF program address
    reti
.org 0x0006                             ; TIMER0_OVF program address
    reti
.org 0x0007                             ; USI_START program address
    reti
.org 0x0008                             ; USI_OVF program address
    rjmp    _INTF_SPI_BYTE_RDY_ISR
.org 0x0009                             ; EE_RDY program address
    reti
.org 0x000A                             ; ANA_COMP program address
    reti
.org 0x000B                             ; ADC program address
    reti
.org 0x000C                             ; WDT program address
    reti
.org 0x000D                             ; INT1 program address
    reti
.org 0x00E                              ; TIMER0_COMPA program address
    reti
.org 0x000F                             ; TIMER0_COMPB program address
    reti
.org 0x0010                             ; TIMER0_CAPT program address
    reti
.org 0x0011                             ; TIMER1_COMPD program address
    reti
.org 0x0012                             ; FAULT_PROTECTION program address
    rjmp    _SVPWM_TIMER_FAULT_ISR


; ==============================================================================
;                      M  A  I  N     P  R  O  G  R  A  M  M
; ==============================================================================
.cseg
INIT:
    wdr                                 ; reset watchdog timer
    ldi     TEMPL,HIGH(RAMEND)          ; set high byte for stack ptr
    out     SPH,TEMPL                   ; init stack ptr high byte
    ldi     TEMPL,LOW(RAMEND)           ; set low byte for stack ptr
    out     SPL,TEMPL                   ; init stack ptr low byte
    ldi     TEMPL,0b00011000            ; initiate watchdog change sequence
    out     WDTCR,TEMPL                 ; start watchdog change sequence
    ldi     TEMPL,0b00000100            ; set new watchdog timeout settings
    out     WDTCR,TEMPL                 ; write new watchdog timeout settings
    ldi     TEMPL,0b00000001
    out     PRR,TEMPL
    sei                                 ; global interrupts enable


SOFT_INIT:
    rcall   SVPWM_INIT                  ;
    rcall   INTF_INIT                   ;


MAIN_LOOP:
    wdr
    rcall   INTF_LOOP
    wdr
    rcall   SVPWM_LOOP
    rjmp    MAIN_LOOP


; ==============================================================================
;                         I  N  T  E  R  F  A  C  E
; ==============================================================================
.include "interface.asm"


; ==============================================================================
;                               S  V  P  W  M
; ==============================================================================
.include "svpwm.asm"
