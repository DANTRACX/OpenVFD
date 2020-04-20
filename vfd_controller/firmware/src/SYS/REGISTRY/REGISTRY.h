#ifndef __REGISTRY_H__
#define __REGISTRY_H__

#include <stdint.h>

typedef struct SETPOINTS_s
{
        int16_t         ENABLE;                             /* P40100 - [-1, 0, 1]        */
        int16_t         TARGET_TORQUE;                      /* P40101 - [-100 < t < 100 ] */
        uint16_t        TARGET_FREQUENCY;                   /* P40102 - [ 0 < f < 65535 ] */
        uint16_t        REVERSAL;                           /* P40103 - [ 0, 1 ]          */
        uint16_t        ENABLE_CURRENT_OVERDRIVE;           /* P40104 - [ 0, 1 ]          */
        uint16_t        ENABLE_FREQUENCY_OVERDRIVE;         /* P40105 - [ 0, 1 ]          */
        uint16_t        ENABLE_PROGMODE;                    /* P40110 - [ 0, 1 ]          */
        uint16_t        TRIGGER_PARAMETER_SAVE;             /* P40111 - [ 0, 1 ]          */
        uint16_t        TRIGGER_UF_CALCULATION;             /* P40120 - [ 0, 1 ]          */
        uint16_t        TRIGGER_RESET;                      /* P40150 - [ 0, 1 ]          */
}
SETPOINTS_s;

typedef struct PROCESSVALUES_s
{
        /* motor process values */
        uint16_t        DCBUS_VOLTAGE;                     /* P30200 - [ V ]             */
        int32_t         DCBUS_CURRENT;                     /* P30201 : P30202 - [ mA ]   */
        int16_t         DCBUS_TEMPERATURE;                 /* P30203 - [ ° ]             */
        uint16_t        MOTOR_FREQUENCY;                   /* P30204 - [ Hz ]            */
        int16_t         MOTOR_POWER;                       /* P30205 - [ W ]             */
        uint16_t        MOTOR_VOLTAGE;                     /* P30206 - [ V ]             */
        int32_t         MOTOR_CURRENT;                     /* P30207 : P30208 - [ mA ]   */
        int16_t         MOTOR_TEMPERATURE;                 /* P30209 - [ ° ]             */

        /* system status */
        uint16_t        SYSTEM_STATUS;                     /* P30250 - [ 0, 1 ]          */

        /* high byte, high nibble - general status */
        #define         SYSTEM_STATUS_ERROR                 0x8000
        #define         SYSTEM_STATUS_RUN                   0x4000
        #define         SYSTEM_STATUS_STOP                  0x2000
        #define         SYSTEM_STATUS_INIT                  0x1000

        /* high byte, low nibble - controller states / umz warnings */
        #define         SYSTEM_STATUS_ENABLE                0x0800
        #define         SYSTEM_STATUS_CURRENT_OVERDRIVE     0x0400
        #define         SYSTEM_STATUS_DCBUS_UMZ_WARNING     0x0200
        #define         SYSTEM_STATUS_MOTOR_UMZ_WARNING     0x0100

        /* low byte - errors */
        #define         SYSTEM_STATUS_DCBUS_TEMP_ERROR      0x0010
        #define         SYSTEM_STATUS_DCBUS_UMZ_ERROR       0x0008
        #define         SYSTEM_STATUS_MOTOR_TEMP_ERROR      0x0004
        #define         SYSTEM_STATUS_MOTOR_UMZ_ERROR       0x0002
        #define         SYSTEM_STATUS_PARAMETER_ERROR       0x0001
}
PROCESSVALUES_s;

typedef struct __attribute__((__packed__)) PARAMETERS_s
{
        /* modbus parameter */
        uint16_t        MODBUS_ADDRESS;                    /* P40300 - [ 1 : 128 ]       */
        uint32_t        MODBUS_BAUDRATE;                   /* P40301 : P40302            */
        uint16_t        MODBUS_PARITY;                     /* P40303 - [ 0, 1, 2 ]       */
        int16_t         MODBUS_OFFSET;                     /* P40304 - [-1, 0, 1 ]       */

        /* motor parameter */
        uint16_t        MOTOR_NOMINAL_VOLTAGE;             /* P40400 - [ V ]             */
        uint16_t        MOTOR_MINIMAL_VOLTAGE;             /* P40401 - [ V ]             */
        uint16_t        MOTOR_NOMINAL_FREQUENCY;           /* P40402 - [ mHz ]           */
        uint16_t        MOTOR_VOLTBOOST_FREQUENCY;         /* P40403 - [ mHz ]           */
        uint16_t        MOTOR_NOMINAL_FWD_CURRENT;         /* P40404 - [ mA ]            */
        uint16_t        MOTOR_NOMINAL_REV_CURRENT;         /* P40405 - [ mA ]            */
        uint16_t        MOTOR_OVERDRIVE_FWD_CURRENT;       /* P40406 - [ mA ]            */
        uint16_t        MOTOR_OVERDRIVE_REV_CURRENT;       /* P40407 - [ mA ]            */

        /* controller parameter */
        uint16_t        CONTROLLER_NOMINAL_FREQUENCY;      /* P40500 - [ mHz ]           */
        uint16_t        CONTROLLER_OVERDRIVE_FREQUENCY;    /* P40501 - [ mHz ]           */
        int16_t         CONTROLLER_PROPORTIONAL_FACTOR;    /* P40502                     */
        int16_t         CONTROLLER_INTEGRAL_FACTOR;        /* P40503                     */
        uint64_t        CONTROLLER_SAMPLETIME;             /* P40504 : P40507 - [ µs ]   */
        uint64_t        CONTROLLER_ENABLE_TIMEOUT;         /* P40508 : P40511 - [ µs ]   */
        uint64_t        CONTROLLER_OVERDRIVE_TIMEOUT;      /* P40512 : P40515 - [ µs ]   */
        uint64_t        CONTROLLER_OVERDRIVE_COOLDOWN;     /* P40516 : P40519 - [ µs ]   */
        uint64_t        CONTROLLER_NOMINAL_UMZ_TIMEOUT;    /* P40520 : P40523 - [ µs ]   */
        uint64_t        CONTROLLER_OVERDRIVE_UMZ_TIMEOUT;  /* P40524 : P40527 - [ µs ]   */
        uint64_t        CONTROLLER_DCBUS_UMZ_TIMEOUT;      /* P40528 : P40531 - [ µs ]   */
        uint16_t        CONTROLLER_DCBUS_NOMINAL_VOLTAGE;  /* P40532 - [ V ]             */
        uint16_t        CONTROLLER_DCBUS_MINIMAL_VOLTAGE;  /* P40533 - [ V ]             */
        uint16_t        CONTROLLER_DCBUS_MAXIMAL_VOLTAGE;  /* P40534 - [ V ]             */
        int16_t         CONTROLLER_DCBUS_VOLTAGE_OFFSET;   /* P40535 - [ V ]             */
        int16_t         CONTROLLER_DCBUS_CURRENT_OFFSET;   /* P40536 - [ mA ]            */
        int16_t         CONTROLLER_DCBUS_FAN_THRESHOLD;    /* P40537 - [ ° ]             */
        int16_t         CONTROLLER_DCBUS_MAXIMAL_TEMP;     /* P40538 - [ ° ]             */
        int16_t         CONTROLLER_MOTOR_FAN_THRESHOLD;    /* P40539 - [ ° ]             */
        int16_t         CONTROLLER_MOTOR_MAXIMAL_TEMP;     /* P40540 - [ ° ]             */
        uint16_t        CONTROLLER_PWM_FREQUENCY;          /* P40541 - [ 0, 1, 2, 3, 4 ] */
        uint16_t        CONTROLLER_PWM_DEADTIME;           /* P40542 - [ 0x00HL ]        */
        uint16_t        CONTROLLER_PWM_DEADTIME_PRESCALING;/* P40543 - [ 0, 1, 2, 3 ]    */

        /* lookup table */
        uint8_t         CONTROLLER_UF_VALUE[2048];         /* P41000 : P43047 ; P43048   */
}
PARAMETERS_s;


extern  SETPOINTS_s     SETPOINTS;
extern  PROCESSVALUES_s PROCESSVALUES;
extern  PARAMETERS_s    PARAMETERS;

extern  void            REGISTRY_INIT(void);
extern  int8_t          REGISTRY_POPEEMEM(void);
extern  void            REGISTRY_PUSHEEMEM(void);
extern  uint8_t         REGISTRY_REGREAD(uint16_t registerAddress, uint16_t *registerValue);
extern  uint8_t         REGISTRY_REGWRITE(uint16_t registerAddress, uint16_t registerValue);

#endif
