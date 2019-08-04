; **********************************************************************
; *          P  R  O  J  E  C  T     T  E  M  P  L  A  T  E            *
; *                                                                    *
; *                        TAB SIZE =  4 SPACES                        *
; **********************************************************************


; ======================================================================
;         D  E  V  I  C  E     D  E  F  I  N  I  T  I  O  N  S
; ======================================================================
.INCLUDE "m16Adef.inc"


; ======================================================================
;      R  E  G  I  S  T  E  R     D  E  F  I  N  I  T  I  O  N  S
; ======================================================================
.def TEMPL      = r16                   ; temp register low byte
.def TEMPH      = r17                   ; temp register high byte
.def COUNTERL   = r18                   ; counter register low byte
.def COUNTERH   = r19                   ; counter register high byte


; ======================================================================
;     V  A  R  I  A  B  L  E     D  E  F  I  N  I  T  I  O  N  S
; ======================================================================
.dseg
    ERROR:                  .byte   1
.cseg


; ======================================================================
;          I  N  T  E  R  R  U  P  T     V  E  C  T  O  R  S
; ======================================================================
.cseg
.org 0x000                              ; RESET VECTOR ADDRESS
    rjmp    INIT                        ; JUMP TO INTERRUPT HANDLER
.org 0x002                              ; INT0 VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x004                              ; INT1 VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x006                              ; TIMER2_COMP VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x008                              ; TIMER2_OVF VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x00A                              ; TIMER1_CAPT VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x00C                              ; TIMER1_COMPA VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x00E                              ; TIMER1_COMPB VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x010                              ; TIMER1_OVF VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x012                              ; TIMER0_OVF VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x014                              ; SPI,STC VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x016                              ; USART,RXC VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x018                              ; USART,UDRE VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x01A                              ; USART_TXC VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x01C                              ; ADC VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x01E                              ; EE_RDY VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x020                              ; ANA_COMP VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x022                              ; TWI VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x024                              ; INT2 VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x026                              ; TIMER0_COMP VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER
.org 0x028                              ; SPM_RDY VECTOR ADDRESS
    reti                                ; JUMP TO INTERRUPT HANDLER


; ======================================================================
;                  M  A  I  N     P  R  O  G  R  A  M  M
; ======================================================================
INIT:
    ldi     TEMPL,HIGH(RAMEND)          ; set high byte for stack ptr
    out     SPH,TEMPL                   ; init stack ptr high byte
    ldi     TEMPL,LOW(RAMEND)           ; set low byte for stack ptr
    out     SPL,TEMPL                   ; init stack ptr low byte
    ldi     COUNTERL,0x00               ; init counter low byte
    ldi     COUNTERH,0x00               ; init counter high byte
    sei                                 ; global interrupts enable

MAIN:

LOOP:                                   ; main programm loop
    rjmp    LOOP                        ; jump to loop



; ======================================================================
;           U  S  B     C  O  M  M  U  N  I  C  A  T  I  O  N
; ======================================================================
;.include "usbcomm.asm"                 ; include additional code