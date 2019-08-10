; ******************************************************************************
; *                A V R   S V P W M   S U B P R O C E S S O R                 *
; *                                 svpwm.asm                                  *
; *                          (C) Christoph Klie 2019                           *
; *                            TAB SIZE =  4 SPACES                            *
; ******************************************************************************
; ==============================================================================
;                               S  V  P  W  M
; ==============================================================================
; This file implements the svpwm algorithm in soft- and hardware. Timer1 with
; its integrated deadtime generator is used.

; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; TODO: - Alle zeilen kommentieren
;       - Multiplikation/Division auf 64bit anpassen
;       - somit tabellengröße von 2406 werten möglich machen


; ==============================================================================
;          R  E  G  I  S  T  E  R     D  E  F  I  N  I  T  I  O  N  S
; ==============================================================================
.def SVPWM_CALC0L   = r0                ; calc 0 register low byte
.def SVPWM_CALC0H   = r1                ; calc 0 register high byte
.def SVPWM_CALC1L   = r2                ; calc 1 register low byte
.def SVPWM_CALC1H   = r3                ; calc 1 register high byte
.def SVPWM_CALC2L   = r4                ; calc 2 register low byte
.def SVPWM_CALC2H   = r5                ; calc 2 register high byte
.def SVPWM_INTVL0L  = r6                ; channel A interval low byte
.def SVPWM_INTVL0H  = r7                ; channel A interval high byte
.def SVPWM_INTVL1L  = r8                ; channel B interval low byte
.def SVPWM_INTVL1H  = r9                ; channel B interval high byte
.def SVPWM_INTVL2L  = r10               ; channel C interval low byte
.def SVPWM_INTVL2H  = r11               ; channel C interval high byte
.def SVPWM_TEMPL    = r16               ; temp register low byte
.def SVPWM_TEMPH    = r17               ; temp register high byte
.def SVPWM_COUNTERL = r18               ; counter register low byte
.def SVPWM_COUNTERH = r19               ; counter register high byte
.def SVPWM_PWMFREQ  = r21               ; holds the desired pwm frequency
.def SVPWM_MAGN     = r22               ; holds the desired magnitude
.def SVPWM_FREQ     = r23               ; holds the desired frequency
.def SVPWM_ANGLEL   = r24               ; low byte of the current angle
.def SVPWM_ANGLEH   = r25               ; high byte of the current angle


; ==============================================================================
;         V  A  R  I  A  B  L  E     D  E  F  I  N  I  T  I  O  N  S
; ==============================================================================
.dseg
.cseg


; ==============================================================================
;       S V P W M  -  U S E R   A C C E S S A B L E   F U N C T I O N S
; ==============================================================================
.cseg
SVPWM_INIT:                             ;
    ldi     SVPWM_MAGN,0xE1             ; init SVPWM_MAGN register
    ldi     SVPWM_FREQ,0x01             ; init SVPWM_FREQ register
    ldi     SVPWM_ANGLEL,0x00           ; init SVPWM_ANGLEL register
    ldi     SVPWM_ANGLEH,0x00           ; init SVPWM_ANGLEH register
    ldi     SVPWM_PWMFREQ,0x10          ;
    rcall   _SVPWM_TIMER_OUTPUTS_SETUP  ;
    rcall   _SVPWM_TIMER_DEADTIME_SETUP ;
    rcall   _SVPWM_TIMER_CTRLREG_SETUP  ;
    rcall   _SVPWM_TIMER_RANGE_SETUP    ;
    rcall   _SVPWM_TIMER_PLL_SETUP      ;
    rcall   _SVPWM_TIMER_PWMOUT_DISABLE ;
    rcall   _SVPWM_CALC_UPDATE_ANGLE
    rcall   _SVPWM_CALC_UPDATE_INTVL
    ret                                 ;


SVPWM_LOOP:                             ;
    ;sbis    PINB,6
    ;ret
update_pwmfreq_1khz:
    cpi     SVPWM_PWMFREQ,0x01
    brne    update_pwmfreq_2khz
    rcall   _SVPWM_TIMER_1KHZ
    rjmp    update_angle
update_pwmfreq_2khz:
    cpi     SVPWM_PWMFREQ,0x02
    brne    update_pwmfreq_4khz
    rcall   _SVPWM_TIMER_2KHZ
    rjmp    update_angle
update_pwmfreq_4khz:
    cpi     SVPWM_PWMFREQ,0x04
    brne    update_pwmfreq_8khz
    rcall   _SVPWM_TIMER_4KHZ
    rjmp    update_angle
update_pwmfreq_8khz:
    cpi     SVPWM_PWMFREQ,0x08
    brne    update_pwmfreq_16khz
    rcall   _SVPWM_TIMER_8KHZ
    rjmp    update_angle
update_pwmfreq_16khz:
    cpi     SVPWM_PWMFREQ,0x10
    brne    update_pwmfreq_stop
    rcall   _SVPWM_TIMER_16KHZ
    rjmp    update_angle
update_pwmfreq_stop:
    rcall   _SVPWM_TIMER_STOP
    ret
update_angle:
    in      SVPWM_TEMPL,TIFR
    sbrs    TEMPL,2
    ret
    rcall   _SVPWM_CALC_UPDATE_ANGLE
    rcall   _SVPWM_CALC_UPDATE_INTVL
    ldi     SVPWM_TEMPL,0b00000100
    out     TIFR,SVPWM_TEMPL
    ret


; ==============================================================================
;      S V P W M  -  H A R D W A R E   G E N E R A T O R   ( T I M E R 1 )
; ==============================================================================
; PLL low speed mode

.cseg
_SVPWM_TIMER_OUTPUTS_SETUP:
    ldi     SVPWM_TEMPL,0b01111111      ; load bit mask in temp
    out     DDRB,SVPWM_TEMPL            ; set i/o to output
    ldi     SVPWM_TEMPL,0b00000000      ; load bit mask in temp
    out     PORTB,SVPWM_TEMPL           ; set all outputs to off
    ret


_SVPWM_TIMER_DEADTIME_SETUP:
    ldi     SVPWM_TEMPL,0xFF            ; dead time cycles (0xHigh:Low)
    out     DT1,SVPWM_TEMPL             ; set dead time
    ret


_SVPWM_TIMER_CTRLREG_SETUP:
    ldi     SVPWM_TEMPL,0b01010011      ; load settings for register
    out     TCCR1A,SVPWM_TEMPL          ; set bit for control register A
    ldi     SVPWM_TEMPL,0b00110000      ; load settings for register
    out     TCCR1B,SVPWM_TEMPL          ; set bit for control register B
    ldi     SVPWM_TEMPL,0b00000000      ; load settings for register
    out     TCCR1C,SVPWM_TEMPL          ; set bit for control register C
    ldi     SVPWM_TEMPL,0b00000001      ; load settings for register
    out     TCCR1D,SVPWM_TEMPL          ; set bit for control register D
    ldi     SVPWM_TEMPL,0b00000000      ; load settings for register
    out     TCCR1E,SVPWM_TEMPL          ; set bit for control register E
    ret


_SVPWM_TIMER_RANGE_SETUP:
    ldi     SVPWM_TEMPL,0xFF            ; define high byte for OCR1C (top)
    ldi     SVPWM_TEMPH,0x03            ; define low byte for OCR1C (top)
    out     TC1H,SVPWM_TEMPH            ; set high byte for OCR1C (top)
    out     OCR1C,SVPWM_TEMPL           ; set low byte for OCR1C (top)
    ret


_SVPWM_TIMER_PLL_SETUP:
    ldi     SVPWM_TEMPL,0b10000010      ; load settings for register
    out     PLLCSR,SVPWM_TEMPL          ; trigger pll start mechanism
    ldi     SVPWM_COUNTERL,0xFF         ; set counter to max value
svpwm_hw_pll_wait:                      ; wait for pll to lock
    nop                                 ; wait
    nop                                 ; wait
    nop                                 ; wait
    dec     SVPWM_COUNTERL              ; decrement counter
    brne    svpwm_hw_pll_wait           ; loop while counter not 0
svpwm_hw_plock_check:                   ; check if plock is set
    in      SVPWM_TEMPL,PLLCSR          ; load pll register data
    sbrs    SVPWM_TEMPL,0               ; check if plock bit is set
    rjmp    svpwm_hw_plock_check        ; if not set goto pll wait
    ldi     SVPWM_TEMPL,0b10000111      ; else load new register data
    out     PLLCSR,SVPWM_TEMPL          ; set pll register - enable pll timer
    ret                                 ; end function


_SVPWM_TIMER_PWMOUT_ENABLE:
    ldi     SVPWM_TEMPL,0b01010101      ; load settings for register
    out     TCCR1C,SVPWM_TEMPL          ; connect outputs to pwm unit
    ret


_SVPWM_TIMER_PWMOUT_DISABLE:
    ldi     SVPWM_TEMPL,0b00000000      ; load settings for register
    out     TCCR1C,SVPWM_TEMPL          ; connect outputs to pwm unit
    ret


_SVPWM_TIMER_STOP:                      ;
    rcall   _SVPWM_TIMER_PWMOUT_DISABLE ;
    in      SVPWM_TEMPL,TCCR1B          ;
    cbr     SVPWM_TEMPL,0b00001111      ;
    out     TCCR1B,SVPWM_TEMPL          ;
    ret                                 ;


_SVPWM_TIMER_1KHZ:
    rcall   _SVPWM_TIMER_PWMOUT_ENABLE  ;
    in      SVPWM_TEMPL,TCCR1B          ;
    cbr     SVPWM_TEMPL,0b00001111      ;
    sbr     SVPWM_TEMPL,0b00000101      ;
    out     TCCR1B,SVPWM_TEMPL          ;
    ret                                 ;


_SVPWM_TIMER_2KHZ:
    rcall   _SVPWM_TIMER_PWMOUT_ENABLE  ;
    in      SVPWM_TEMPL,TCCR1B          ;
    cbr     SVPWM_TEMPL,0b00001111      ;
    sbr     SVPWM_TEMPL,0b00000100      ;
    out     TCCR1B,SVPWM_TEMPL          ;
    ret                                 ;


_SVPWM_TIMER_4KHZ:                      ;
    rcall   _SVPWM_TIMER_PWMOUT_ENABLE  ;
    in      SVPWM_TEMPL,TCCR1B          ;
    cbr     SVPWM_TEMPL,0b00001111      ;
    sbr     SVPWM_TEMPL,0b00000011      ;
    out     TCCR1B,SVPWM_TEMPL          ;
    ret                                 ;


_SVPWM_TIMER_8KHZ:                      ;
    rcall   _SVPWM_TIMER_PWMOUT_ENABLE  ;
    in      SVPWM_TEMPL,TCCR1B          ;
    cbr     SVPWM_TEMPL,0b00001111      ;
    sbr     SVPWM_TEMPL,0b00000010      ;
    out     TCCR1B,SVPWM_TEMPL          ;
    ret                                 ;


_SVPWM_TIMER_16KHZ:                     ;
    rcall   _SVPWM_TIMER_PWMOUT_ENABLE  ;
    in      SVPWM_TEMPL,TCCR1B          ;
    cbr     SVPWM_TEMPL,0b00001111      ;
    sbr     SVPWM_TEMPL,0b00000001      ;
    out     TCCR1B,SVPWM_TEMPL          ;
    ret


_SVPWM_TIMER_FAULT_ISR:                 ;
    rcall   _SVPWM_TIMER_STOP           ;
    reti                                ;


; ==============================================================================
;            S V P W M   -   S O F T W A R E   C A L C U L A T I O N
; ==============================================================================
.cseg
_SVPWM_CALC_UPDATE_ANGLE:               ;
    in      SVPWM_TEMPL,TCCR1B          ; setup counter for anglestep correction
    cbr     SVPWM_TEMPL,0b11111000
    sbrc    SVPWM_FREQ,7
    rjmp    svpwm_calc_load_neg_freq
svpwm_calc_load_pos_freq:
    ldi     SVPWM_COUNTERH,0x00         ;
    mov     SVPWM_COUNTERL,SVPWM_FREQ   ;
    rjmp    svpwm_calc_angelstep_corr
svpwm_calc_load_neg_freq:
    ldi     SVPWM_COUNTERH,0x00         ;
    ldi     SVPWM_COUNTERL,0xFF
    SUB     SVPWM_COUNTERL,SVPWM_FREQ
    inc     SVPWM_COUNTERL
svpwm_calc_angelstep_corr:              ;
    dec     SVPWM_TEMPL
    breq    svpwm_calc_angle_dir_sel
    lsl     SVPWM_COUNTERL              ;
    rol     SVPWM_COUNTERH              ;
    rjmp    svpwm_calc_angelstep_corr   ;
svpwm_calc_angle_dir_sel:               ; decide for rotation direction
    sbrc    SVPWM_FREQ,7                ;
    rjmp    svpwm_calc_angle_dec        ;
svpwm_calc_angle_inc:                   ;
    add     SVPWM_ANGLEL,SVPWM_COUNTERL ;
    adc     SVPWM_ANGLEH,SVPWM_COUNTERH ;
    ldi     SVPWM_TEMPL,0x20            ; set TEMP registers to 15624
    ldi     SVPWM_TEMPH,0x1C            ; set TEMP registers to 15624
    cp      SVPWM_ANGLEL,SVPWM_TEMPL    ; check if ANGLE >= 15624
    cpc     SVPWM_ANGLEH,SVPWM_TEMPH    ; check if ANGLE >= 15624
    brsh    svpwm_calc_angle_inc_ovf    ; if ANGLE >= 15624 : jump
    ret                                 ; else return
svpwm_calc_angle_inc_ovf:               ;
    sub     SVPWM_ANGLEL,SVPWM_TEMPL    ; subtract 15624 from ANGLE
    sbc     SVPWM_ANGLEH,SVPWM_TEMPH    ; subtract 15624 from ANGLE
    ret                                 ; return
svpwm_calc_angle_dec:                   ;
    sub     SVPWM_ANGLEL,SVPWM_COUNTERL ;
    sbc     SVPWM_ANGLEH,SVPWM_COUNTERH ;
    ldi     SVPWM_TEMPL,0x20            ;
    ldi     SVPWM_TEMPH,0x1C            ;
    cp      SVPWM_ANGLEL,SVPWM_TEMPL    ;
    cpc     SVPWM_ANGLEH,SVPWM_TEMPH    ;
    brsh    svpwm_calc_angle_dec_ovf    ;
    ret                                 ;
svpwm_calc_angle_dec_ovf:               ;
    add     SVPWM_ANGLEL,SVPWM_TEMPL    ;
    adc     SVPWM_ANGLEH,SVPWM_TEMPH    ;
    ret                                 ;


_SVPWM_CALC_UPDATE_INTVL:               ;
    ldi     SVPWM_COUNTERH,0x00         ;
    mov     SVPWM_CALC0L,SVPWM_ANGLEL   ;
    mov     SVPWM_CALC0H,SVPWM_ANGLEH   ;
    ldi     SVPWM_TEMPL,0x00            ;
    ldi     SVPWM_TEMPH,0x04            ;
svpwm_calc_intvl_sector:                ;
    cp      SVPWM_CALC0L,SVPWM_TEMPL    ;
    cpc     SVPWM_CALC0H,SVPWM_TEMPH    ;
    brlo    svpwm_calc_intvl_t1         ;
    inc     SVPWM_COUNTERH              ;
    sub     SVPWM_CALC0L,SVPWM_TEMPL    ;
    sbc     SVPWM_CALC0H,SVPWM_TEMPH    ;
    rjmp    svpwm_calc_intvl_sector     ;
svpwm_calc_intvl_t1:                    ;
    sub     SVPWM_TEMPL,SVPWM_CALC0L    ;
    sbc     SVPWM_TEMPH,SVPWM_CALC0H    ;
    lsl     SVPWM_TEMPL                 ;
    rol     SVPWM_TEMPH                 ;
    ldi     ZL,LOW(SINE*2)              ;
    ldi     ZH,HIGH(SINE*2)             ;
    add     ZL,SVPWM_TEMPL              ;
    adc     ZH,SVPWM_TEMPH              ;
    lpm     SVPWM_INTVL1L,Z+            ;
    lpm     SVPWM_INTVL1H,Z             ;
svpwm_calc_intvl_t2:                    ;
    lsl     SVPWM_CALC0L                ;
    rol     SVPWM_CALC0H                ;
    ldi     ZL,LOW(SINE*2)              ;
    ldi     ZH,HIGH(SINE*2)             ;
    add     ZL,SVPWM_CALC0L             ;
    adc     ZH,SVPWM_CALC0H             ;
    lpm     SVPWM_INTVL2L,Z+            ;
    lpm     SVPWM_INTVL2H,Z             ;
svpwm_calc_mult_t1t2:                   ;
    mov     SVPWM_COUNTERL,SVPWM_MAGN   ;
    ldi     SVPWM_TEMPL,0x00            ;
    ldi     SVPWM_TEMPH,0x00            ;
    clr     SVPWM_CALC0L                ; clear t1 result byte 0
    clr     SVPWM_CALC1L                ; clear t1 result byte 1
    clr     SVPWM_CALC2L                ; clear t1 result byte 2
    clr     SVPWM_CALC0H                ; clear t2 result byte 0
    clr     SVPWM_CALC1H                ; clear t2 result byte 1
    clr     SVPWM_CALC2H                ; clear t2 result byte 2
svpwm_calc_mult_t1t2_a:                 ;
    clc                                 ;
    ror     SVPWM_COUNTERL              ;
    brcc    svpwm_calc_mult_t1t2_b      ;
    add     SVPWM_CALC0L,SVPWM_INTVL1L  ;
    adc     SVPWM_CALC1L,SVPWM_INTVL1H  ;
    adc     SVPWM_CALC2L,SVPWM_TEMPL    ;
    add     SVPWM_CALC0H,SVPWM_INTVL2L  ;
    adc     SVPWM_CALC1H,SVPWM_INTVL2H  ;
    adc     SVPWM_CALC2H,SVPWM_TEMPH    ;
svpwm_calc_mult_t1t2_b:                 ;
    clc                                 ;
    rol     SVPWM_INTVL1L               ;
    rol     SVPWM_INTVL1H               ;
    rol     SVPWM_TEMPL                 ;
    clc                                 ;
    rol     SVPWM_INTVL2L               ;
    rol     SVPWM_INTVL2H               ;
    rol     SVPWM_TEMPH                 ;
    tst     SVPWM_COUNTERL              ;
    brne    svpwm_calc_mult_t1t2_a      ;
svpwm_calc_div_t1t2:                    ;
    mov     SVPWM_INTVL1L,SVPWM_CALC1L  ;
    mov     SVPWM_INTVL1H,SVPWM_CALC2L  ;
    mov     SVPWM_INTVL2L,SVPWM_CALC1H  ;
    mov     SVPWM_INTVL2H,SVPWM_CALC2H  ;
svpwm_calc_intvl_t0:                    ;
    mov     SVPWM_CALC0L,SVPWM_INTVL1L  ;
    mov     SVPWM_CALC0H,SVPWM_INTVL1H  ;
    add     SVPWM_CALC0L,SVPWM_INTVL2L  ;
    adc     SVPWM_CALC0H,SVPWM_INTVL2H  ;
    ldi     SVPWM_TEMPL,0x00            ;
    ldi     SVPWM_TEMPH,0x04            ;
    sub     SVPWM_TEMPL,SVPWM_CALC0L    ;
    sbc     SVPWM_TEMPH,SVPWM_CALC0H    ;
    lsr     SVPWM_TEMPH                 ;
    ror     SVPWM_TEMPL                 ;
    mov     SVPWM_INTVL0L,SVPWM_TEMPL   ;
    mov     SVPWM_INTVL0H,SVPWM_TEMPH   ;
svpwm_calc_intvl_setcomp:               ;
    ldi     SVPWM_TEMPL,0x00            ;
    ldi     ZL,low(_SVPWM_CALC_JTB)     ;
    ldi     ZH,high(_SVPWM_CALC_JTB)    ;
    add     ZL,SVPWM_COUNTERH           ;
    adc     ZH,SVPWM_TEMPL              ;
    ijmp                                ;


_SVPWM_CALC_JTB:                        ;
    rjmp    _SVPWM_CALC_SETCOMP_SECTOR0 ;
    rjmp    _SVPWM_CALC_SETCOMP_SECTOR1 ;
    rjmp    _SVPWM_CALC_SETCOMP_SECTOR2 ;
    rjmp    _SVPWM_CALC_SETCOMP_SECTOR3 ;
    rjmp    _SVPWM_CALC_SETCOMP_SECTOR4 ;
    rjmp    _SVPWM_CALC_SETCOMP_SECTOR5 ;
    ret                                 ;


_SVPWM_CALC_SETCOMP_SECTOR0:            ;
    out     TC1H,SVPWM_INTVL0H          ;
    out     OCR1A,SVPWM_INTVL0L         ;
    add     SVPWM_INTVL0L,SVPWM_INTVL1L ;
    adc     SVPWM_INTVL0H,SVPWM_INTVL1H ;
    out     TC1H,SVPWM_INTVL0H          ;
    out     OCR1B,SVPWM_INTVL0L         ;
    add     SVPWM_INTVL0L,SVPWM_INTVL2L ;
    adc     SVPWM_INTVL0H,SVPWM_INTVL2H ;
    out     TC1H,SVPWM_INTVL0H          ;
    out     OCR1D,SVPWM_INTVL0L         ;
    ret                                 ;


_SVPWM_CALC_SETCOMP_SECTOR1:            ;
    out     TC1H,SVPWM_INTVL0H          ;
    out     OCR1B,SVPWM_INTVL0L         ;
    add     SVPWM_INTVL0L,SVPWM_INTVL2L ;
    adc     SVPWM_INTVL0H,SVPWM_INTVL2H ;
    out     TC1H,SVPWM_INTVL0H          ;
    out     OCR1A,SVPWM_INTVL0L         ;
    add     SVPWM_INTVL0L,SVPWM_INTVL1L ;
    adc     SVPWM_INTVL0H,SVPWM_INTVL1H ;
    out     TC1H,SVPWM_INTVL0H          ;
    out     OCR1D,SVPWM_INTVL0L         ;
    ret                                 ;


_SVPWM_CALC_SETCOMP_SECTOR2:            ;
    out     TC1H,SVPWM_INTVL0H          ;
    out     OCR1B,SVPWM_INTVL0L         ;
    add     SVPWM_INTVL0L,SVPWM_INTVL1L ;
    adc     SVPWM_INTVL0H,SVPWM_INTVL1H ;
    out     TC1H,SVPWM_INTVL0H          ;
    out     OCR1D,SVPWM_INTVL0L         ;
    add     SVPWM_INTVL0L,SVPWM_INTVL2L ;
    adc     SVPWM_INTVL0H,SVPWM_INTVL2H ;
    out     TC1H,SVPWM_INTVL0H          ;
    out     OCR1A,SVPWM_INTVL0L         ;
    ret                                 ;


_SVPWM_CALC_SETCOMP_SECTOR3:            ;
    out     TC1H,SVPWM_INTVL0H          ;
    out     OCR1D,SVPWM_INTVL0L         ;
    add     SVPWM_INTVL0L,SVPWM_INTVL2L ;
    adc     SVPWM_INTVL0H,SVPWM_INTVL2H ;
    out     TC1H,SVPWM_INTVL0H          ;
    out     OCR1B,SVPWM_INTVL0L         ;
    add     SVPWM_INTVL0L,SVPWM_INTVL1L ;
    adc     SVPWM_INTVL0H,SVPWM_INTVL1H ;
    out     TC1H,SVPWM_INTVL0H          ;
    out     OCR1A,SVPWM_INTVL0L         ;
    ret                                 ;


_SVPWM_CALC_SETCOMP_SECTOR4:            ;
    out     TC1H,SVPWM_INTVL0H          ;
    out     OCR1D,SVPWM_INTVL0L         ;
    add     SVPWM_INTVL0L,SVPWM_INTVL1L ;
    adc     SVPWM_INTVL0H,SVPWM_INTVL1H ;
    out     TC1H,SVPWM_INTVL0H          ;
    out     OCR1A,SVPWM_INTVL0L         ;
    add     SVPWM_INTVL0L,SVPWM_INTVL2L ;
    adc     SVPWM_INTVL0H,SVPWM_INTVL2H ;
    out     TC1H,SVPWM_INTVL0H          ;
    out     OCR1B,SVPWM_INTVL0L         ;
    ret                                 ;


_SVPWM_CALC_SETCOMP_SECTOR5:            ;
    out     TC1H,SVPWM_INTVL0H          ;
    out     OCR1A,SVPWM_INTVL0L         ;
    add     SVPWM_INTVL0L,SVPWM_INTVL2L ;
    adc     SVPWM_INTVL0H,SVPWM_INTVL2H ;
    out     TC1H,SVPWM_INTVL0H          ;
    out     OCR1D,SVPWM_INTVL0L         ;
    add     SVPWM_INTVL0L,SVPWM_INTVL1L ;
    adc     SVPWM_INTVL0H,SVPWM_INTVL1H ;
    out     TC1H,SVPWM_INTVL0H          ;
    out     OCR1B,SVPWM_INTVL0L         ;
    ret                                 ;


; ==============================================================================
;    S V P W M  -  S I N E   L O O K U P   T A B L E   0 - 6 0   D E G R E E
; ==============================================================================
; (8 x 325) + 3 = 2604 Values / 60 Degrees = 15624 Values / 360 Degrees
.cseg
SINE:
    .dw     0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0004, 0x0005, 0x0006
    .dw     0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000D
    .dw     0x000E, 0x000F, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015
    .dw     0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001A, 0x001B, 0x001C
    .dw     0x001D, 0x001D, 0x001E, 0x001F, 0x0020, 0x0021, 0x0022, 0x0023
    .dw     0x0024, 0x0025, 0x0026, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A
    .dw     0x002B, 0x002C, 0x002D, 0x002E, 0x002E, 0x002F, 0x0030, 0x0031
    .dw     0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0036, 0x0037, 0x0038
    .dw     0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F, 0x003F
    .dw     0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047
    .dw     0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E
    .dw     0x004F, 0x004F, 0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055
    .dw     0x0056, 0x0057, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C
    .dw     0x005D, 0x005E, 0x005F, 0x005F, 0x0060, 0x0061, 0x0062, 0x0063
    .dw     0x0064, 0x0065, 0x0066, 0x0067, 0x0067, 0x0068, 0x0069, 0x006A
    .dw     0x006B, 0x006C, 0x006D, 0x006E, 0x006F, 0x006F, 0x0070, 0x0071
    .dw     0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0077, 0x0078
    .dw     0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F, 0x007F
    .dw     0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087
    .dw     0x0087, 0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E
    .dw     0x008F, 0x008F, 0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095
    .dw     0x0096, 0x0096, 0x0097, 0x0098, 0x0099, 0x009A, 0x009B, 0x009C
    .dw     0x009D, 0x009E, 0x009E, 0x009F, 0x00A0, 0x00A1, 0x00A2, 0x00A3
    .dw     0x00A4, 0x00A5, 0x00A5, 0x00A6, 0x00A7, 0x00A8, 0x00A9, 0x00AA
    .dw     0x00AB, 0x00AC, 0x00AD, 0x00AD, 0x00AE, 0x00AF, 0x00B0, 0x00B1
    .dw     0x00B2, 0x00B3, 0x00B4, 0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00B8
    .dw     0x00B9, 0x00BA, 0x00BB, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF
    .dw     0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C3, 0x00C4, 0x00C5, 0x00C6
    .dw     0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CA, 0x00CB, 0x00CC, 0x00CD
    .dw     0x00CE, 0x00CF, 0x00D0, 0x00D1, 0x00D1, 0x00D2, 0x00D3, 0x00D4
    .dw     0x00D5, 0x00D6, 0x00D7, 0x00D8, 0x00D8, 0x00D9, 0x00DA, 0x00DB
    .dw     0x00DC, 0x00DD, 0x00DE, 0x00DF, 0x00DF, 0x00E0, 0x00E1, 0x00E2
    .dw     0x00E3, 0x00E4, 0x00E5, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9
    .dw     0x00EA, 0x00EB, 0x00EC, 0x00EC, 0x00ED, 0x00EE, 0x00EF, 0x00F0
    .dw     0x00F1, 0x00F2, 0x00F3, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7
    .dw     0x00F8, 0x00F9, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE
    .dw     0x00FF, 0x0100, 0x0100, 0x0101, 0x0102, 0x0103, 0x0104, 0x0105
    .dw     0x0106, 0x0106, 0x0107, 0x0108, 0x0109, 0x010A, 0x010B, 0x010C
    .dw     0x010C, 0x010D, 0x010E, 0x010F, 0x0110, 0x0111, 0x0112, 0x0113
    .dw     0x0113, 0x0114, 0x0115, 0x0116, 0x0117, 0x0118, 0x0119, 0x0119
    .dw     0x011A, 0x011B, 0x011C, 0x011D, 0x011E, 0x011F, 0x011F, 0x0120
    .dw     0x0121, 0x0122, 0x0123, 0x0124, 0x0125, 0x0125, 0x0126, 0x0127
    .dw     0x0128, 0x0129, 0x012A, 0x012B, 0x012B, 0x012C, 0x012D, 0x012E
    .dw     0x012F, 0x0130, 0x0131, 0x0131, 0x0132, 0x0133, 0x0134, 0x0135
    .dw     0x0136, 0x0136, 0x0137, 0x0138, 0x0139, 0x013A, 0x013B, 0x013C
    .dw     0x013C, 0x013D, 0x013E, 0x013F, 0x0140, 0x0141, 0x0142, 0x0142
    .dw     0x0143, 0x0144, 0x0145, 0x0146, 0x0147, 0x0147, 0x0148, 0x0149
    .dw     0x014A, 0x014B, 0x014C, 0x014D, 0x014D, 0x014E, 0x014F, 0x0150
    .dw     0x0151, 0x0152, 0x0152, 0x0153, 0x0154, 0x0155, 0x0156, 0x0157
    .dw     0x0158, 0x0158, 0x0159, 0x015A, 0x015B, 0x015C, 0x015D, 0x015D
    .dw     0x015E, 0x015F, 0x0160, 0x0161, 0x0162, 0x0162, 0x0163, 0x0164
    .dw     0x0165, 0x0166, 0x0167, 0x0167, 0x0168, 0x0169, 0x016A, 0x016B
    .dw     0x016C, 0x016C, 0x016D, 0x016E, 0x016F, 0x0170, 0x0171, 0x0171
    .dw     0x0172, 0x0173, 0x0174, 0x0175, 0x0176, 0x0176, 0x0177, 0x0178
    .dw     0x0179, 0x017A, 0x017B, 0x017B, 0x017C, 0x017D, 0x017E, 0x017F
    .dw     0x0180, 0x0180, 0x0181, 0x0182, 0x0183, 0x0184, 0x0185, 0x0185
    .dw     0x0186, 0x0187, 0x0188, 0x0189, 0x018A, 0x018A, 0x018B, 0x018C
    .dw     0x018D, 0x018E, 0x018E, 0x018F, 0x0190, 0x0191, 0x0192, 0x0193
    .dw     0x0193, 0x0194, 0x0195, 0x0196, 0x0197, 0x0197, 0x0198, 0x0199
    .dw     0x019A, 0x019B, 0x019C, 0x019C, 0x019D, 0x019E, 0x019F, 0x01A0
    .dw     0x01A0, 0x01A1, 0x01A2, 0x01A3, 0x01A4, 0x01A5, 0x01A5, 0x01A6
    .dw     0x01A7, 0x01A8, 0x01A9, 0x01A9, 0x01AA, 0x01AB, 0x01AC, 0x01AD
    .dw     0x01AE, 0x01AE, 0x01AF, 0x01B0, 0x01B1, 0x01B2, 0x01B2, 0x01B3
    .dw     0x01B4, 0x01B5, 0x01B6, 0x01B6, 0x01B7, 0x01B8, 0x01B9, 0x01BA
    .dw     0x01BA, 0x01BB, 0x01BC, 0x01BD, 0x01BE, 0x01BE, 0x01BF, 0x01C0
    .dw     0x01C1, 0x01C2, 0x01C2, 0x01C3, 0x01C4, 0x01C5, 0x01C6, 0x01C7
    .dw     0x01C7, 0x01C8, 0x01C9, 0x01CA, 0x01CB, 0x01CB, 0x01CC, 0x01CD
    .dw     0x01CE, 0x01CE, 0x01CF, 0x01D0, 0x01D1, 0x01D2, 0x01D2, 0x01D3
    .dw     0x01D4, 0x01D5, 0x01D6, 0x01D6, 0x01D7, 0x01D8, 0x01D9, 0x01DA
    .dw     0x01DA, 0x01DB, 0x01DC, 0x01DD, 0x01DE, 0x01DE, 0x01DF, 0x01E0
    .dw     0x01E1, 0x01E2, 0x01E2, 0x01E3, 0x01E4, 0x01E5, 0x01E5, 0x01E6
    .dw     0x01E7, 0x01E8, 0x01E9, 0x01E9, 0x01EA, 0x01EB, 0x01EC, 0x01ED
    .dw     0x01ED, 0x01EE, 0x01EF, 0x01F0, 0x01F0, 0x01F1, 0x01F2, 0x01F3
    .dw     0x01F4, 0x01F4, 0x01F5, 0x01F6, 0x01F7, 0x01F7, 0x01F8, 0x01F9
    .dw     0x01FA, 0x01FB, 0x01FB, 0x01FC, 0x01FD, 0x01FE, 0x01FE, 0x01FF
    .dw     0x0200, 0x0201, 0x0202, 0x0202, 0x0203, 0x0204, 0x0205, 0x0205
    .dw     0x0206, 0x0207, 0x0208, 0x0208, 0x0209, 0x020A, 0x020B, 0x020C
    .dw     0x020C, 0x020D, 0x020E, 0x020F, 0x020F, 0x0210, 0x0211, 0x0212
    .dw     0x0212, 0x0213, 0x0214, 0x0215, 0x0216, 0x0216, 0x0217, 0x0218
    .dw     0x0219, 0x0219, 0x021A, 0x021B, 0x021C, 0x021C, 0x021D, 0x021E
    .dw     0x021F, 0x021F, 0x0220, 0x0221, 0x0222, 0x0222, 0x0223, 0x0224
    .dw     0x0225, 0x0225, 0x0226, 0x0227, 0x0228, 0x0228, 0x0229, 0x022A
    .dw     0x022B, 0x022B, 0x022C, 0x022D, 0x022E, 0x022E, 0x022F, 0x0230
    .dw     0x0231, 0x0231, 0x0232, 0x0233, 0x0234, 0x0234, 0x0235, 0x0236
    .dw     0x0237, 0x0237, 0x0238, 0x0239, 0x023A, 0x023A, 0x023B, 0x023C
    .dw     0x023D, 0x023D, 0x023E, 0x023F, 0x0240, 0x0240, 0x0241, 0x0242
    .dw     0x0243, 0x0243, 0x0244, 0x0245, 0x0245, 0x0246, 0x0247, 0x0248
    .dw     0x0248, 0x0249, 0x024A, 0x024B, 0x024B, 0x024C, 0x024D, 0x024E
    .dw     0x024E, 0x024F, 0x0250, 0x0250, 0x0251, 0x0252, 0x0253, 0x0253
    .dw     0x0254, 0x0255, 0x0256, 0x0256, 0x0257, 0x0258, 0x0258, 0x0259
    .dw     0x025A, 0x025B, 0x025B, 0x025C, 0x025D, 0x025E, 0x025E, 0x025F
    .dw     0x0260, 0x0260, 0x0261, 0x0262, 0x0263, 0x0263, 0x0264, 0x0265
    .dw     0x0265, 0x0266, 0x0267, 0x0268, 0x0268, 0x0269, 0x026A, 0x026A
    .dw     0x026B, 0x026C, 0x026D, 0x026D, 0x026E, 0x026F, 0x026F, 0x0270
    .dw     0x0271, 0x0271, 0x0272, 0x0273, 0x0274, 0x0274, 0x0275, 0x0276
    .dw     0x0276, 0x0277, 0x0278, 0x0279, 0x0279, 0x027A, 0x027B, 0x027B
    .dw     0x027C, 0x027D, 0x027D, 0x027E, 0x027F, 0x0280, 0x0280, 0x0281
    .dw     0x0282, 0x0282, 0x0283, 0x0284, 0x0284, 0x0285, 0x0286, 0x0287
    .dw     0x0287, 0x0288, 0x0289, 0x0289, 0x028A, 0x028B, 0x028B, 0x028C
    .dw     0x028D, 0x028D, 0x028E, 0x028F, 0x028F, 0x0290, 0x0291, 0x0292
    .dw     0x0292, 0x0293, 0x0294, 0x0294, 0x0295, 0x0296, 0x0296, 0x0297
    .dw     0x0298, 0x0298, 0x0299, 0x029A, 0x029A, 0x029B, 0x029C, 0x029C
    .dw     0x029D, 0x029E, 0x029E, 0x029F, 0x02A0, 0x02A0, 0x02A1, 0x02A2
    .dw     0x02A2, 0x02A3, 0x02A4, 0x02A5, 0x02A5, 0x02A6, 0x02A7, 0x02A7
    .dw     0x02A8, 0x02A9, 0x02A9, 0x02AA, 0x02AB, 0x02AB, 0x02AC, 0x02AD
    .dw     0x02AD, 0x02AE, 0x02AF, 0x02AF, 0x02B0, 0x02B1, 0x02B1, 0x02B2
    .dw     0x02B2, 0x02B3, 0x02B4, 0x02B4, 0x02B5, 0x02B6, 0x02B6, 0x02B7
    .dw     0x02B8, 0x02B8, 0x02B9, 0x02BA, 0x02BA, 0x02BB, 0x02BC, 0x02BC
    .dw     0x02BD, 0x02BE, 0x02BE, 0x02BF, 0x02C0, 0x02C0, 0x02C1, 0x02C2
    .dw     0x02C2, 0x02C3, 0x02C3, 0x02C4, 0x02C5, 0x02C5, 0x02C6, 0x02C7
    .dw     0x02C7, 0x02C8, 0x02C9, 0x02C9, 0x02CA, 0x02CB, 0x02CB, 0x02CC
    .dw     0x02CC, 0x02CD, 0x02CE, 0x02CE, 0x02CF, 0x02D0, 0x02D0, 0x02D1
    .dw     0x02D2, 0x02D2, 0x02D3, 0x02D3, 0x02D4, 0x02D5, 0x02D5, 0x02D6
    .dw     0x02D7, 0x02D7, 0x02D8, 0x02D8, 0x02D9, 0x02DA, 0x02DA, 0x02DB
    .dw     0x02DC, 0x02DC, 0x02DD, 0x02DD, 0x02DE, 0x02DF, 0x02DF, 0x02E0
    .dw     0x02E1, 0x02E1, 0x02E2, 0x02E2, 0x02E3, 0x02E4, 0x02E4, 0x02E5
    .dw     0x02E6, 0x02E6, 0x02E7, 0x02E7, 0x02E8, 0x02E9, 0x02E9, 0x02EA
    .dw     0x02EA, 0x02EB, 0x02EC, 0x02EC, 0x02ED, 0x02EE, 0x02EE, 0x02EF
    .dw     0x02EF, 0x02F0, 0x02F1, 0x02F1, 0x02F2, 0x02F2, 0x02F3, 0x02F4
    .dw     0x02F4, 0x02F5, 0x02F5, 0x02F6, 0x02F7, 0x02F7, 0x02F8, 0x02F8
    .dw     0x02F9, 0x02FA, 0x02FA, 0x02FB, 0x02FB, 0x02FC, 0x02FD, 0x02FD
    .dw     0x02FE, 0x02FE, 0x02FF, 0x0300, 0x0300, 0x0301, 0x0301, 0x0302
    .dw     0x0302, 0x0303, 0x0304, 0x0304, 0x0305, 0x0305, 0x0306, 0x0307
    .dw     0x0307, 0x0308, 0x0308, 0x0309, 0x0309, 0x030A, 0x030B, 0x030B
    .dw     0x030C, 0x030C, 0x030D, 0x030E, 0x030E, 0x030F, 0x030F, 0x0310
    .dw     0x0310, 0x0311, 0x0312, 0x0312, 0x0313, 0x0313, 0x0314, 0x0314
    .dw     0x0315, 0x0316, 0x0316, 0x0317, 0x0317, 0x0318, 0x0318, 0x0319
    .dw     0x031A, 0x031A, 0x031B, 0x031B, 0x031C, 0x031C, 0x031D, 0x031D
    .dw     0x031E, 0x031F, 0x031F, 0x0320, 0x0320, 0x0321, 0x0321, 0x0322
    .dw     0x0323, 0x0323, 0x0324, 0x0324, 0x0325, 0x0325, 0x0326, 0x0326
    .dw     0x0327, 0x0327, 0x0328, 0x0329, 0x0329, 0x032A, 0x032A, 0x032B
    .dw     0x032B, 0x032C, 0x032C, 0x032D, 0x032D, 0x032E, 0x032F, 0x032F
    .dw     0x0330, 0x0330, 0x0331, 0x0331, 0x0332, 0x0332, 0x0333, 0x0333
    .dw     0x0334, 0x0334, 0x0335, 0x0336, 0x0336, 0x0337, 0x0337, 0x0338
    .dw     0x0338, 0x0339, 0x0339, 0x033A, 0x033A, 0x033B, 0x033B, 0x033C
    .dw     0x033C, 0x033D, 0x033D, 0x033E, 0x033F, 0x033F, 0x0340, 0x0340
    .dw     0x0341, 0x0341, 0x0342, 0x0342, 0x0343, 0x0343, 0x0344, 0x0344
    .dw     0x0345, 0x0345, 0x0346, 0x0346, 0x0347, 0x0347, 0x0348, 0x0348
    .dw     0x0349, 0x0349, 0x034A, 0x034A, 0x034B, 0x034B, 0x034C, 0x034C
    .dw     0x034D, 0x034D, 0x034E, 0x034E, 0x034F, 0x034F, 0x0350, 0x0350
    .dw     0x0351, 0x0351, 0x0352, 0x0352, 0x0353, 0x0353, 0x0354, 0x0354
    .dw     0x0355, 0x0355, 0x0356, 0x0356, 0x0357, 0x0357, 0x0358, 0x0358
    .dw     0x0359, 0x0359, 0x035A, 0x035A, 0x035B, 0x035B, 0x035C, 0x035C
    .dw     0x035D, 0x035D, 0x035E, 0x035E, 0x035F, 0x035F, 0x0360, 0x0360
    .dw     0x0361, 0x0361, 0x0362, 0x0362, 0x0363, 0x0363, 0x0363, 0x0364
    .dw     0x0364, 0x0365, 0x0365, 0x0366, 0x0366, 0x0367, 0x0367, 0x0368
    .dw     0x0368, 0x0369, 0x0369, 0x036A, 0x036A, 0x036B, 0x036B, 0x036B
    .dw     0x036C, 0x036C, 0x036D, 0x036D, 0x036E, 0x036E, 0x036F, 0x036F
    .dw     0x0370, 0x0370, 0x0370, 0x0371, 0x0371, 0x0372, 0x0372, 0x0373
    .dw     0x0373, 0x0374, 0x0374, 0x0375, 0x0375, 0x0375, 0x0376, 0x0376
