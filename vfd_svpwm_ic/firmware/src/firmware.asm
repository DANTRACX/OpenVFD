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
;.def CALC0L     = r0
;.def CALC0H     = r1
;.def CALC1L     = r2
;.def CALC1H     = r3
;.def CALC2L     = r4
;.def CALC2H     = r5
.def TEMPL      = r16                   ; temp register low byte
.def TEMPH      = r17                   ; temp register high byte
;.def COUNTERL   = r18                   ; counter register low byte
;.def COUNTERH   = r19                   ; counter register high byte


; ==============================================================================
;         V  A  R  I  A  B  L  E     D  E  F  I  N  I  T  I  O  N  S
; ==============================================================================
.dseg
    ERROR:                  .byte   1

; ==============================================================================
;              I  N  T  E  R  R  U  P  T     V  E  C  T  O  R  S
; ==============================================================================
.cseg
.org 0x000                              ; RESET program address
    rjmp    INIT
.org 0x001                              ; INT0 program address
    reti
.org 0x002                              ; PCINT vecprogramtor address
    reti
.org 0x003                              ; TIMER1_COMPA program address
    reti
.org 0x004                              ; TIMER1_COMPB program address
    reti
.org 0x005                              ; TIMER1_OVF program address
    reti
.org 0x006                              ; TIMER0_OVF program address
    reti
.org 0x007                              ; USI_START program address
    reti
.org 0x008                              ; USI_OVF program address
    reti
.org 0x009                              ; EE_RDY program address
    reti
.org 0x00A                              ; ANA_COMP program address
    reti
.org 0x00B                              ; ADC program address
    reti
.org 0x00C                              ; WDT program address
    reti
.org 0x00D                              ; INT1 program address
    reti
.org 0x00E                              ; TIMER0_COMPA program address
    reti
.org 0x00F                              ; TIMER0_COMPB program address
    reti
.org 0x010                              ; TIMER0_CAPT program address
    reti
.org 0x011                              ; TIMER1_COMPD program address
    reti
.org 0x012                              ; FAULT_PROTECTION program address
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
    sei                                 ; global interrupts enable

SOFT_INIT:
    rcall   SVPWM_INIT                  ;


MAIN_LOOP:
    wdr
    rcall   SVPWM_LOOP
    rjmp    MAIN_LOOP


; ==============================================================================
;                               S  V  P  W  M
; ==============================================================================
.include "svpwm.asm"

; ==============================================================================
;                         I  N  T  E  R  F  A  C  E
; ==============================================================================
;.include "interface.asm"
