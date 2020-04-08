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
}
SETPOINTS_s;

typedef struct PROCESSVALUES_s
{
        /* motor process values */
        int16_t         DCBUS_VOLTAGE;                     /* P30200 - [ V ]             */
        int16_t         DCBUS_CURRENT;                     /* P30201 - [ mA ]            */
        int16_t         MOTOR_VOLTAGE;                     /* P30202 - [ V ]             */
        int16_t         MOTOR_CURRENT;                     /* P30203 - [ mA ]            */
        uint16_t        MOTOR_FREQUENCY;                   /* P30204 - [ Hz ]            */
        int16_t         MOTOR_POWER;                       /* P30205 - [ W ]             */

        /* system status */
        char            SYSTEM_STATUS[50];                 /* P30210 : P30248 - [ASCII]  */
        uint16_t        SYSTEM_ERROR;                      /* P30250 - [ 0, 1 ]          */
}
PROCESSVALUES_s;

typedef struct __attribute__((__packed__)) PARAMETERS_s
{
        /* modbus parameter */
        uint16_t        MODBUS_ADDRESS;                    /* P40300 - [1 : 128]         */

        /* pwm parameter */
        uint16_t        PWM_FREQUENCY;                     /* P40400 - [ 0, 1, 2, 3, 4 ] */
        uint16_t        PWM_DEADTIME;                      /* P40401 - [ 0x00HL ]        */
        uint16_t        PWM_DEADTIME_PRESCALING;           /* P40402 - [ 0, 1, 2, 3 ]    */

        /* motor parameter */
        uint16_t        DCBUS_NOMINAL_VOLTAGE;             /* P40500 - [ V ]             */
        uint16_t        MOTOR_NOMINAL_VOLTAGE;             /* P40501 - [ V ]             */
        uint16_t        MOTOR_MINIMAL_VOLTAGE;             /* P40502 - [ V ]             */
        uint16_t        MOTOR_NOMINAL_FREQUENCY;           /* P40503 - [ mHz ]           */
        uint16_t        MOTOR_VOLTBOOST_FREQUENCY;         /* P40504 - [ mHz ]           */
        uint16_t        MOTOR_NOMINAL_FWD_CURRENT;         /* P40505 - [ mA ]            */
        uint16_t        MOTOR_NOMINAL_REV_CURRENT;         /* P40506 - [ mA ]            */
        uint16_t        MOTOR_OVERDRIVE_FWD_CURRENT;       /* P40507 - [ mA ]            */
        uint16_t        MOTOR_OVERDRIVE_REV_CURRENT;       /* P40508 - [ mA ]            */

        /* controller parameter */
        uint16_t        CONTROLLER_NOMINAL_FREQUENCY;      /* P40600 - [ mHz ]           */
        uint16_t        CONTROLLER_OVERDRIVE_FREQUENCY;    /* P40601 - [ mHz ]           */
        int16_t         CONTROLLER_PROPORTIONAL_FACTOR;    /* P40602                     */
        int16_t         CONTROLLER_INTEGRAL_FACTOR;        /* P40603                     */
        uint64_t        CONTROLLER_SAMPLETIME;             /* P40604 : P40611 - [ µs ]   */
        uint64_t        CONTROLLER_ENABLE_TIMEOUT;         /* P40612 : P40619 - [ µs ]   */
        uint64_t        CONTROLLER_OVERDRIVE_TIMEOUT;      /* P40620 : P40627 - [ µs ]   */
        uint64_t        CONTROLLER_OVERDRIVE_COOLDOWN;     /* P40628 : P40635 - [ µs ]   */
        uint8_t         CONTROLLER_UF_VALUE[2048];         /* P41000 : P43047 ; P43048   */
}
PARAMETERS_s;


extern  SETPOINTS_s     SETPOINTS;
extern  PROCESSVALUES_s PROCESSVALUES;
extern  PARAMETERS_s    PARAMETERS;

extern  void            REGISTRY_INIT(void);
extern  void            REGISTRY_SETSTATUS(char *msg, uint8_t size);
extern  int8_t          REGISTRY_POPEEMEM(void);
extern  void            REGISTRY_PUSHEEMEM(void);
extern  uint8_t         REGISTRY_REGREAD(uint16_t registerAddress, int64_t *registerValue);
extern  void            REGISTRY_REGWRITE(uint16_t registerAddress, int64_t registerValue);

#endif
