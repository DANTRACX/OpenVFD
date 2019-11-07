; ******************************************************************************
; *                A V R   S V P W M   S U B P R O C E S S O R                 *
; *                               interface.asm                                *
; *                          (C) Christoph Klie 2019                           *
; *                            TAB SIZE =  4 SPACES                            *
; ******************************************************************************
; ==============================================================================
;                        I  N  T  E  R  F  A  C  E
; ==============================================================================
; This file implements the communication interface between the main processor
; and the svpwm_subprocessor.


; ==============================================================================
;          R  E  G  I  S  T  E  R     D  E  F  I  N  I  T  I  O  N  S
; ==============================================================================
.def INTF_TEMPL     = r16               ; temp register low byte
.def INTF_TEMPH     = r17               ; temp register high byte
.def INTF_COUNTERL  = r18               ; counter register low byte
.def INTF_COUNTERH  = r19               ; counter register high byte
.def INTF_MAGN      = r20               ; holds the desired magnitude
.def INTF_FREQL     = r21               ; holds the desired frequency
.def INTF_FREQH     = r22               ; holds the desired frequency
.def INTF_PHASEL    = r23               ; low byte of the current angle
.def INTF_PHASEH    = r24               ; high byte of the current angle
.def INTF_ESREG     = r25               ;
.equ INTF_EESREG    = EEARL             ;
.equ INTF_DEADTIME  = GPIOR2            ;


;INTF_ESREG BIT DEFINITIONS
;———————————————————————————————————————————————————————————————————————————————
;    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0
;  CW/CCW | DATARDY |  BUF1/2 |  DTPS1  |  DTPS0  |  FPWM2  |  FPWM1  |  FPWM0
;———————————————————————————————————————————————————————————————————————————————


; CMD : 0x01 - set rotation direction - bytes following : 1
;———————————————————————————————————————————————————————————————————————————————
;    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0
;    -    |    -    |    -    |    -    |    -    |    -    |    -    |   SRD
;———————————————————————————————————————————————————————————————————————————————


; CMD : 0x02 - set voltage magnitude - bytes following : 1
;———————————————————————————————————————————————————————————————————————————————
;    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0
;   MSB   |         |         |         |         |         |         |   LSB
;———————————————————————————————————————————————————————————————————————————————


; CMD : 0x03 - set voltage frequency - bytes following : 2
;———————————————————————————————————————————————————————————————————————————————
;    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0
;                                  FREQL [7:0]
;———————————————————————————————————————————————————————————————————————————————
;———————————————————————————————————————————————————————————————————————————————
;    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0
;                                  FREQH [7:0]
;———————————————————————————————————————————————————————————————————————————————


; CMD : 0x04 - set voltage phase - bytes following : 2
;———————————————————————————————————————————————————————————————————————————————
;    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0
;                                  PHASEL [7:0]
;———————————————————————————————————————————————————————————————————————————————
;———————————————————————————————————————————————————————————————————————————————
;    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0
;                                  PHASEH [7:0]
;———————————————————————————————————————————————————————————————————————————————


; CMD : 0x05 - set pwm frequency - bytes following : 1
;———————————————————————————————————————————————————————————————————————————————
;    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0
;    -    |    -    |    -    |    -    |    -    |  FPWM2  |  FPWM1  |  FPWM0
;———————————————————————————————————————————————————————————————————————————————


; CMD : 0x06 - set deadtime values - bytes following : 1
;———————————————————————————————————————————————————————————————————————————————
;    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0
;   DTh3  |   DTh2  |   DTh1  |   DTh0  |   DTl3  |   DTl2  |   DTl1  |   DTl0
;———————————————————————————————————————————————————————————————————————————————


; CMD : 0x07 - set deadtime prescaler - bytes following : 1
;———————————————————————————————————————————————————————————————————————————————
;    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0
;    -    |    -    |    -    |    -    |    -    |    -    |  DTPS1  |  DTPS0
;———————————————————————————————————————————————————————————————————————————————


; ==============================================================================
;         V  A  R  I  A  B  L  E     D  E  F  I  N  I  T  I  O  N  S
; ==============================================================================
.dseg
    INTF_BUFF1:         .byte   128
    INTF_BUFF2:         .byte   128
.cseg


; ==============================================================================
;    I N T E R F A C E  -  U S E R   A C C E S S A B L E   F U N C T I O N S
; ==============================================================================
.cseg
INTF_INIT:                              ;
    rcall   _INTF_SPI_BUFFER_SETUP      ;
    rcall   _INTF_SPI_OUTPUTS_SETUP     ;
    rcall   _INTF_SPI_CTRLREG_SETUP     ;
    ret                                 ;


INTF_LOOP:                              ;
    sbrs    INTF_ESREG,6                ;
    ret                                 ;
    ldi     INTF_COUNTERL,0x20          ;
intf_loop_assign_process:               ;
    dec     INTF_COUNTERL               ;
    breq    intf_loop_exit              ;
    ld      INTF_TEMPL,X+               ;
    cpi     INTF_TEMPL,0x00             ;
    breq    intf_loop_exit              ;
    cpi     INTF_TEMPL,0x01             ;
    breq    intf_loop_rotation          ;
    cpi     INTF_TEMPL,0x02             ;
    breq    intf_loop_magn              ;
    cpi     INTF_TEMPL,0x03             ;
    breq    intf_loop_freq              ;
    cpi     INTF_TEMPL,0x04             ;
    breq    intf_loop_phase             ;
    cpi     INTF_TEMPL,0x05             ;
    breq    intf_loop_pwmfreq           ;
    cpi     INTF_TEMPL,0x06             ;
    breq    intf_loop_deadtime          ;
    cpi     INTF_TEMPL,0x07             ;
    breq    intf_loop_deadtime_prsc     ;
    rjmp    intf_loop_exit              ;
intf_loop_rotation:                     ;
    ld      INTF_TEMPL,X+               ;
    cbr     INTF_TEMPL,0b01111111       ;
    cbr     INTF_ESREG,0b10000000       ;
    or      INTF_ESREG,INTF_TEMPL       ;
    rjmp    intf_loop_assign_process    ;
intf_loop_magn:                         ;
    ld      INTF_TEMPL,X+               ;
    mov     INTF_MAGN,INTF_TEMPL        ;
    rjmp    intf_loop_assign_process    ;
intf_loop_freq:                         ;
    ld      INTF_TEMPL,X+               ;
    mov     INTF_FREQL,INTF_TEMPL       ;
    ld      INTF_TEMPL,X+               ;
    mov     INTF_FREQH,INTF_TEMPL       ;
    dec     INTF_COUNTERL               ;
    rjmp    intf_loop_assign_process    ;
intf_loop_phase:                        ;
    ld      INTF_TEMPL,X+               ;
    mov     INTF_PHASEL,INTF_TEMPL      ;
    ld      INTF_TEMPL,X+               ;
    mov     INTF_PHASEH,INTF_TEMPL      ;
    dec     INTF_COUNTERL               ;
    rjmp    intf_loop_assign_process    ;
intf_loop_pwmfreq:                      ;
    ld      INTF_TEMPL,X+               ;
    cbr     INTF_TEMPL,0b11111000       ;
    cbr     INTF_ESREG,0b00000111       ;
    or      INTF_ESREG,INTF_TEMPL       ;
    sbi     INTF_EESREG,0               ;
    rjmp    intf_loop_assign_process    ;
intf_loop_deadtime:                     ;
    ld      INTF_TEMPL,X+               ;
    out     INTF_DEADTIME,INTF_TEMPL    ;
    sbi     INTF_EESREG,0               ;
    rjmp    intf_loop_assign_process    ;
intf_loop_deadtime_prsc:                ;
    ld      INTF_TEMPL,X+               ;
    cbr     INTF_ESREG,0b00011000       ;
    sbrc    INTF_ESREG,3                ;
    sbr     INTF_ESREG,0b00001000       ;
    sbrc    INTF_ESREG,4                ;
    sbr     INTF_ESREG,0b00010000       ;
    sbi     INTF_EESREG,0               ;
    rjmp    intf_loop_assign_process    ;
intf_loop_exit:                         ;
    cbr     INTF_ESREG,0b01000000       ;
    ret                                 ;


; ==============================================================================
;              U  S  I    -    S  P  I     I  N  T  E  R  F  A  C  E
; ==============================================================================
.cseg
_INTF_SPI_OUTPUTS_SETUP:                ;
    in      INTF_TEMPL,DDRA             ;
    sbr     INTF_TEMPL,0b00000010       ;
    cbr     INTF_TEMPL,0b00001101       ;
    out     DDRA,INTF_TEMPL             ;
    in      INTF_TEMPL,PORTA            ;
    cbr     INTF_TEMPL,0b00001111       ;
    out     PORTA,INTF_TEMPL            ;
    ret                                 ;


_INTF_SPI_CTRLREG_SETUP:                ;
    ldi     INTF_TEMPL,0b00000001       ;
    out     USIPP,INTF_TEMPL            ;
    ldi     INTF_TEMPL,0b01010000       ;
    out     USICR,INTF_TEMPL            ;
    ldi     INTF_TEMPL,0b00000000       ;
    out     USISR,INTF_TEMPL            ;
    ldi     INTF_TEMPL,0b00001000       ;
    out     PCMSK0,INTF_TEMPL           ;
    ldi     INTF_TEMPL,0b00000000       ;
    out     PCMSK1,INTF_TEMPL           ;
    in      INTF_TEMPL,GIMSK            ;
    sbr     INTF_TEMPL,0b00110000       ;
    out     GIMSK,INTF_TEMPL            ;
    ret                                 ;


_INTF_SPI_BUFFER_SETUP:                 ;
    ldi     YL,low(INTF_BUFF1)          ;
    ldi     YH,high(INTF_BUFF1)         ;
    ldi     XL,low(INTF_BUFF2)          ;
    ldi     XH,high(INTF_BUFF2)         ;
    cbr     INTF_ESREG,0x00             ;
    ldi     INTF_TEMPL,0x00             ;
    ldi     INTF_COUNTERL,0x7F          ;
intf_spi_buffer_setup_loop:             ;
    st      X+,INTF_TEMPL               ;
    st      Y+,INTF_TEMPL               ;
    dec     INTF_COUNTERL               ;
    brne    intf_spi_buffer_setup_loop  ;
    ldi     YL,low(INTF_BUFF1)          ;
    ldi     YH,high(INTF_BUFF1)         ;
    ldi     XL,low(INTF_BUFF2)          ;
    ldi     XH,high(INTF_BUFF2)         ;
    ret                                 ;


_INTF_SPI_BYTE_RDY_ISR:                 ;
    push    INTF_TEMPL                  ;
    in      INTF_TEMPL,USIDR            ;
    st      Y+,INTF_TEMPL               ;
    pop     INTF_TEMPL                  ;
    sbi     USISR,6                     ;
    reti                                ;


_INTF_SPI_SS_ISR:                       ;
    sbis    PINA,3                      ;
    rjmp    intf_spi_isr_pinL           ;
intf_spi_isr_pinH:                      ;
    cbi     USICR,3                     ;
    push    INTF_TEMPL                  ;
    ldi     INTF_TEMPL,0b01000000       ;
    out     USISR,INTF_TEMPL            ;
    ldi     INTF_TEMPL,0x00             ;
    st      Y,INTF_TEMPL                ;
    pop     INTF_TEMPL                  ;
    sbrc    INTF_ESREG,6
    rjmp    intf_spi_isr_pinh_sw00
    sbrc    INTF_ESREG,5                ;
    rjmp    intf_spi_isr_pinh_sw21      ;
intf_spi_isr_pinh_sw12:                 ;
    sbr     INTF_ESREG,0b00100000       ;
    ldi     YL,low(INTF_BUFF2)          ;
    ldi     YH,high(INTF_BUFF2)         ;
    ldi     XL,low(INTF_BUFF1)          ;
    ldi     XH,high(INTF_BUFF1)         ;
    sbr     INTF_ESREG,0b01000000       ;
    reti                                ;
intf_spi_isr_pinh_sw21:                 ;
    cbr     INTF_ESREG,0b00100000       ;
    ldi     YL,low(INTF_BUFF1)          ;
    ldi     YH,high(INTF_BUFF1)         ;
    ldi     XL,low(INTF_BUFF2)          ;
    ldi     XH,high(INTF_BUFF2)         ;
    sbr     INTF_ESREG,0b01000000       ;
    reti                                ;
intf_spi_isr_pinh_sw00:                 ;
    sbrc    INTF_ESREG,5                ;
    rjmp    intf_spi_isr_pinh_sw12      ;
    rjmp    intf_spi_isr_pinh_sw21      ;
intf_spi_isr_pinL:                      ;
    sbi     USICR,3                     ;
    reti                                ;


; ==============================================================================
;             U  S  I    -     I  2  C     I  N  T  E  R  F  A  C  E
; ==============================================================================
; not yet implemented - no neccessity so far
