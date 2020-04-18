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
        uint16_t        MOTOR_FREQUENCY;                   /* P30205 - [ Hz ]            */
        int16_t         MOTOR_POWER;                       /* P30206 - [ W ]             */
        uint16_t        MOTOR_VOLTAGE;                     /* P30207 - [ V ]             */
        int32_t         MOTOR_CURRENT;                     /* P30208 : P30209 - [ mA ]   */

        /* system status */
        uint16_t        SYSTEM_STATUS;                     /* P30250 - [ 0, 1 ]          */

        /* high byte, high nibble - general status */
        #define         SYSTEM_STATUS_ERROR                 0x8000
        #define         SYSTEM_STATUS_WARNING               0x4000
        #define         SYSTEM_STATUS_ENABLE                0x2000
        #define         SYSTEM_STATUS_CURRENT_OVERDRIVE     0x1000

        /* high byte, low nibble - system state */
        #define         SYSTEM_STATUS_RUN                   0x0400
        #define         SYSTEM_STATUS_STOP                  0x0200
        #define         SYSTEM_STATUS_INIT                  0x0100

        /* low byte, high nibble - warnings */
        #define         SYSTEM_STATUS_DCBUS_UMZ_WARNING     0x0010
        #define         SYSTEM_STATUS_MOTOR_UMZ_WARNING     0x0020

        /* low byte, low nibble - errors */
        #define         SYSTEM_STATUS_DCBUS_UMZ_ERROR       0x0001
        #define         SYSTEM_STATUS_MOTOR_UMZ_ERROR       0x0002
        #define         SYSTEM_STATUS_PARAMETER_ERROR       0x0008
}
PROCESSVALUES_s;

typedef struct __attribute__((__packed__)) PARAMETERS_s
{
        /* modbus parameter */
        uint16_t        MODBUS_ADDRESS;                    /* P40300 - [1 : 128 ]        */
        uint32_t        MODBUS_BAUDRATE;                   /* P40301 : P40302            */
        uint16_t        MODBUS_PARITY;                     /* P40303 - [0, 1, 2 ]        */

        /* pwm parameter */
        uint16_t        PWM_FREQUENCY;                     /* P40400 - [ 0, 1, 2, 3, 4 ] */
        uint16_t        PWM_DEADTIME;                      /* P40401 - [ 0x00HL ]        */
        uint16_t        PWM_DEADTIME_PRESCALING;           /* P40402 - [ 0, 1, 2, 3 ]    */

        /* motor parameter */
        uint16_t        DCBUS_NOMINAL_VOLTAGE;             /* P40500 - [ V ]             */
        uint16_t        DCBUS_MINIMAL_VOLTAGE;             /* P40501 - [ V ]             */
        uint16_t        DCBUS_MAXIMAL_VOLTAGE;             /* P40502 - [ V ]             */
        uint16_t        MOTOR_NOMINAL_VOLTAGE;             /* P40503 - [ V ]             */
        uint16_t        MOTOR_MINIMAL_VOLTAGE;             /* P40504 - [ V ]             */
        uint16_t        MOTOR_NOMINAL_FREQUENCY;           /* P40505 - [ mHz ]           */
        uint16_t        MOTOR_VOLTBOOST_FREQUENCY;         /* P40506 - [ mHz ]           */
        uint16_t        MOTOR_NOMINAL_FWD_CURRENT;         /* P40507 - [ mA ]            */
        uint16_t        MOTOR_NOMINAL_REV_CURRENT;         /* P40508 - [ mA ]            */
        uint16_t        MOTOR_OVERDRIVE_FWD_CURRENT;       /* P40509 - [ mA ]            */
        uint16_t        MOTOR_OVERDRIVE_REV_CURRENT;       /* P40510 - [ mA ]            */

        /* controller parameter */
        uint16_t        CONTROLLER_NOMINAL_FREQUENCY;      /* P40600 - [ mHz ]           */
        uint16_t        CONTROLLER_OVERDRIVE_FREQUENCY;    /* P40601 - [ mHz ]           */
        int16_t         CONTROLLER_PROPORTIONAL_FACTOR;    /* P40602                     */
        int16_t         CONTROLLER_INTEGRAL_FACTOR;        /* P40603                     */
        uint64_t        CONTROLLER_SAMPLETIME;             /* P40604 : P40607 - [ µs ]   */
        uint64_t        CONTROLLER_ENABLE_TIMEOUT;         /* P40608 : P40611 - [ µs ]   */
        uint64_t        CONTROLLER_OVERDRIVE_TIMEOUT;      /* P40612 : P40615 - [ µs ]   */
        uint64_t        CONTROLLER_OVERDRIVE_COOLDOWN;     /* P40616 : P40619 - [ µs ]   */
        uint64_t        CONTROLLER_NOMINAL_UMZ_TIMEOUT;    /* P40620 : P40623 - [ µs ]   */
        uint64_t        CONTROLLER_OVERDRIVE_UMZ_TIMEOUT;  /* P40624 : P40627 - [ µs ]   */
        uint64_t        CONTROLLER_DCBUS_UMZ_TIMEOUT;      /* P40628 : P40631 - [ µs ]   */
        int32_t         CONTROLLER_DCBUS_VOLTAGE_OFFSET;   /* P40632 : P40633 - [ V ]    */
        int32_t         CONTROLLER_DCBUS_CURRENT_OFFSET;   /* P40634 : P40635 - [ mA ]   */
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
extern  void            REGISTRY_REGWRITE(uint16_t registerAddress, uint16_t registerValue);

#endif
