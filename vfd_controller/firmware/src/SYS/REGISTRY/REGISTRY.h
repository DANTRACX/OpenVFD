#ifndef __REGISTRY_H__
#define __REGISTRY_H__

#include <stdint.h>

typedef struct SETPOINTS_s
{
    int16_t  TIMED_ENABLE;
    uint16_t TARGET_FREQUENCY;
    int16_t  RELATIVE_TORQUE;
    uint16_t REVERSAL;
    uint16_t ENABLE_FREQUENCY_OVERDRIVE;
    uint16_t ENABLE_CURRENT_OVERDRIVE;
    uint16_t ENABLE_PROGMODE;
}
SETPOINTS_s;

typedef struct MEASUREMENTS_s
{
    int16_t  LINE_VOLTAGE;      /*  [V]  */
    int16_t  LINE_CURRENT;      /*  [mA] */
    int16_t  BUS_VOLTAGE;       /*  [V]  */
    int16_t  BUS_CURRENT;       /*  [mA] */
    uint16_t OUTPUT_FREQUENCY;  /* [mHz] */
    int16_t  OUTPUT_POWER;      /*  [W]  */
    int16_t  TEMPERATURE1;      /*  [C]  */
    int16_t  TEMPERATURE2;      /*  [C]  */
}
MEASUREMENTS_s;

typedef struct PARAMETERS_s
{
    uint16_t NOMINAL_FREQUENCY;
    uint16_t OVERDRIVE_FREQUENCY;
    uint16_t NOMINAL_FORWARD_LINE_CURRENT;
    uint16_t NOMINAL_REVERSE_LINE_CURRENT;
    uint16_t OVERDRIVE_FORWARD_LINE_CURRENT;
    uint16_t OVERDRIVE_REVERSE_LINE_CURRENT;
    uint8_t  PWM_FREQUENCY;
    uint64_t ENABLE_TIMEOUT;
    uint64_t OVERDRIVE_TIMEOUT;
    uint64_t OVERDRIVE_COOLDOWN;
    uint64_t CONTROLLER_SAMPLETIME;
    int16_t  CONTROLLER_P_VALUE;
    int16_t  CONTROLLER_I_VALUE;
    uint16_t CONTROLLER_UF_VALUE[1024];
    char     SYSTEM_STATUS[100];
}
PARAMETERS_s;


extern SETPOINTS_s SETPOINTS;
extern MEASUREMENTS_s MEASUREMENTS;
extern PARAMETERS_s PARAMETERS;

extern void REGISTRY_INIT(void);
extern void REGISTRY_READ(void);
extern void REGISTRY_UPDATE(void);

#endif
