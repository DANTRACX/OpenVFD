#ifndef __REGISTRY_H__
#define __REGISTRY_H__

#include <stdint.h>

typedef struct SETPOINTS_s
{
    int16_t  ENABLE;                        /* P40100 - [-1, 0, 1]        */
    uint16_t ENABLE_FREQUENCY_OVERDRIVE;    /* P40101 - [ 0, 1 ]          */
    uint16_t ENABLE_CURRENT_OVERDRIVE;      /* P40102 - [ 0, 1 ]          */
    uint16_t ENABLE_PROGMODE;               /* P40103 - [ 0, 1 ]          */
    uint16_t TARGET_FREQUENCY;              /* P40104 - [ 0 < f < 65535 ] */
    int16_t  TARGET_TORQUE;                 /* P40105 - [-100 < t < 100 ] */
    uint16_t REVERSAL;                      /* P40106 - [ 0, 1 ]          */
}
SETPOINTS_s;

typedef struct PROCESSVALUES_s
{
    /* motor process values */
    int16_t  DCBUS_VOLTAGE;                 /* P30200 - [ V ]             */
    int16_t  DCBUS_CURRENT;                 /* P30201 - [ mA ]            */
    int16_t  MOTOR_VOLTAGE;                 /* P30202 - [ V ]             */
    int16_t  MOTOR_CURRENT;                 /* P30203 - [ mA ]            */
    uint16_t MOTOR_FREQUENCY;               /* P30204 - [ Hz ]            */
    int16_t  MOTOR_POWER;                   /* P30205 - [ W ]             */

    /* controller process values */
    uint8_t  CONTROLLER_UF_VALUE[1024];

    /* system status */
    char     SYSTEM_STATUS[50];             /* P30210 : P30260 - [ASCII]  */
}
PROCESSVALUES_s;

typedef struct PARAMETERS_s
{
    /* motor parameter */
    uint16_t DCBUS_NOMINAL_VOLTAGE;         /* P40300 - [ V ]             */
    uint16_t MOTOR_NOMINAL_VOLTAGE;         /* P40301 - [ V ]             */
    uint16_t MOTOR_MINIMAL_VOLTAGE;         /* P40302 - [ V ]             */
    uint16_t MOTOR_NOMINAL_FREQUENCY;       /* P40303 - [ mHz ]           */
    uint16_t MOTOR_VOLTBOOST_FREQUENCY;     /* P40304 - [ mHz ]           */
    uint16_t MOTOR_NOMINAL_FWD_CURRENT;     /* P40305 - [ mA ]            */
    uint16_t MOTOR_NOMINAL_REV_CURRENT;     /* P40306 - [ mA ]            */
    uint16_t MOTOR_OVERDRIVE_FWD_CURRENT;   /* P40307 - [ mA ]            */
    uint16_t MOTOR_OVERDRIVE_REV_CURRENT;   /* P40308 - [ mA ]            */

    /* controller parameter */
    uint16_t CONTROLLER_NOMINAL_FREQUENCY;  /* P40310 - [ mHz ]           */
    uint16_t CONTROLLER_OVERDRIVE_FREQUENCY;/* P40311 - [ mHz ]           */
    int16_t  CONTROLLER_PROPORTIONAL_FACTOR;/* P40312                     */
    int16_t  CONTROLLER_INTEGRAL_FACTOR;    /* P40313                     */
    uint64_t CONTROLLER_SAMPLETIME;         /* P40314 : P40321 - [ µs ]   */
    uint64_t CONTROLLER_ENABLE_TIMEOUT;     /* P40322 : P40329 - [ µs ]   */
    uint64_t CONTROLLER_OVERDRIVE_TIMEOUT;  /* P40330 : P40337 - [ µs ]   */
    uint64_t CONTROLLER_OVERDRIVE_COOLDOWN; /* P40338 : P40345 - [ µs ]   */


    /* pwm parameter */
    uint16_t PWM_FREQUENCY;                 /* P40350 - [ 0, 1, 2, 3, 4 ] */
    uint16_t PWM_DEADTIME;                  /* P40351 - [ 0x00HL ]        */
    uint16_t PWM_DEADTIME_PRESCALING;       /* P40352 - [ 0, 1, 2, 3 ]    */

    /* modbus parameter */
    uint16_t MODBUS_ADDRESS;                /* P40360 - [1 : 128]         */
}
PARAMETERS_s;


extern SETPOINTS_s SETPOINTS;
extern PROCESSVALUES_s PROCESSVALUES;
extern PARAMETERS_s PARAMETERS;

extern void REGISTRY_INIT(void);
extern void REGISTRY_READ(void);
extern void REGISTRY_UPDATE(void);

#endif
