#include "CONTROLLER.h"
#include "./MAVG/MAVG.h"
#include "../REGISTRY/REGISTRY.h"
#include "../../RES/SVPWM/SVPWM.h"
#include "../../RES/SENSE/SENSE.h"
#include "../../RES/TIME/TIME.h"

static struct CONTROLLERSTATES_s
{
    uint8_t enable;
    int64_t error;
    int64_t ierror;
    uint8_t currentOverdrive;
    TIME_s stepCycleTimer;
    TIME_s overdriveTimer;
    TIME_s enableTimer;
    MAVG_s filtered_buscurrent;
    MAVG_s filtered_busvoltage;
}
CONTROLLERSTATES;


__attribute__((always_inline))
static inline void _controller_check_enable(void)
{
    uint8_t expired = TIME_CHECKEXP(&(CONTROLLERSTATES.enableTimer));

    /* enable flag 0 - normal operation until timer timeout */
    if(SETPOINTS.TIMED_ENABLE == 0)
    {
        /* check timeout - disable controller if expired */
        if(expired)
        {
            CONTROLLERSTATES.enable = 0;
            MEASUREMENTS.OUTPUT_FREQUENCY = 0;
            SVPWM_STOP();
            return;
        }
    }

    /* enable flag 1 - normal/startup operation until timer timeout */
    else if(SETPOINTS.TIMED_ENABLE == 1)
    {
        /* reset enable timer */
        SETPOINTS.TIMED_ENABLE = 0;
        TIME_SET(&(CONTROLLERSTATES.enableTimer), PARAMETERS.ENABLE_TIMEOUT);

        /* check if controller was previously disabled - reactivate it */
        if(CONTROLLERSTATES.enable == 0)
        {
            CONTROLLERSTATES.enable = 1;
            CONTROLLERSTATES.error = 0;
            CONTROLLERSTATES.ierror = 0;

            if(SETPOINTS.REVERSAL == 1)
            {
                SVPWM_QUEUE_SET_ROTATION_DIRECTION(COUNTERCLOCKWISE);
            }

            else
            {
                SVPWM_QUEUE_SET_ROTATION_DIRECTION(CLOCKWISE);
            }

            SVPWM_QUEUE_SET_FREQUENCY(0x0000);
            SVPWM_QUEUE_SET_MAGNITUDE(0x00);
            SVPWM_QUEUE_SEND();
            __builtin_avr_delay_cycles(20000);
            SVPWM_START();
        }
    }

    /* emergency stop for any other value */
    else
    {
        SETPOINTS.TIMED_ENABLE = -1;
        CONTROLLERSTATES.enable = 0;
        MEASUREMENTS.OUTPUT_FREQUENCY = 0;
        SVPWM_STOP();
        return;
    }
}

__attribute__((always_inline))
static inline void _controller_check_overdrive(void)
{
    uint8_t expired = TIME_CHECKEXP(&(CONTROLLERSTATES.overdriveTimer));

    /* reset overdrive, switch to cooldown */
    if((expired) && (CONTROLLERSTATES.currentOverdrive == 1))
    {
        /* disable overdrive, start cooldown */
        CONTROLLERSTATES.currentOverdrive = 0;
        SETPOINTS.ENABLE_CURRENT_OVERDRIVE = 0;
        TIME_SET(&(CONTROLLERSTATES.overdriveTimer), PARAMETERS.OVERDRIVE_COOLDOWN);
    }

    /* reset cooldown after timeout */
    else if((expired) && (CONTROLLERSTATES.currentOverdrive == 0))
    {
        /* enable overdrive, start timeout */
        if(SETPOINTS.ENABLE_CURRENT_OVERDRIVE == 1)
        {
            CONTROLLERSTATES.currentOverdrive = 1;
            TIME_SET(&(CONTROLLERSTATES.overdriveTimer), PARAMETERS.OVERDRIVE_TIMEOUT);
        }
    }
}

__attribute__((always_inline))
static inline void _controller_check_overload(void)
{

}


void CONTROLLER_INIT(void)
{
    CONTROLLERSTATES.enable = 0;
    CONTROLLERSTATES.error = 0;
    CONTROLLERSTATES.ierror = 0;
    CONTROLLERSTATES.currentOverdrive = 0;

    MAVG_INIT(&(CONTROLLERSTATES.filtered_busvoltage));
    MAVG_INIT(&(CONTROLLERSTATES.filtered_buscurrent));
    TIME_INIT();
    TIME_SET(&(CONTROLLERSTATES.stepCycleTimer), PARAMETERS.CONTROLLER_SAMPLETIME);
    TIME_SET(&(CONTROLLERSTATES.overdriveTimer), PARAMETERS.OVERDRIVE_TIMEOUT);
    TIME_SET(&(CONTROLLERSTATES.enableTimer), 1);
    SENSE_INIT();
    SVPWM_INIT();
    SVPWM_STOP();
    SVPWM_QUEUE_SET_PWM_FREQUENCY(KHZ_8);
    SVPWM_QUEUE_SET_MAGNITUDE(0);
    SVPWM_QUEUE_SET_FREQUENCY(0);
    SVPWM_QUEUE_SEND();
}

void CONTROLLER_STEP_CYCLE(void)
{
    /* declare some working variables */
    int64_t reference_current = 0;
    int64_t feedback_current = (int64_t)(MEASUREMENTS.BUS_CURRENT);
    int64_t feedback_voltage = (int64_t)(MEASUREMENTS.BUS_VOLTAGE);
    int64_t antiwindup_ierror = CONTROLLERSTATES.ierror;
    int64_t proportional = 0;
    int64_t integral = 0;
    int64_t output = 0;


    /* calculate depending measurement values */
    /* approximation of sqrt(2) by (1448/1024) and 1/sqrt(2) by (1448/2028) */
    feedback_voltage = ((((feedback_voltage * 1448) / 2048) * ((PARAMETERS.CONTROLLER_UF_VALUE[(MEASUREMENTS.OUTPUT_FREQUENCY) >> 6]))) >> 8);
    feedback_current = ((feedback_current * 1448) / 2048);

    MEASUREMENTS.LINE_VOLTAGE = (uint16_t)feedback_voltage;
    MEASUREMENTS.LINE_CURRENT = (int16_t)feedback_current;


    /* check for enable and overdrive */
    _controller_check_enable();
    _controller_check_overload();
    _controller_check_overdrive();


    /* check if controller calculation is necessary */
    if(CONTROLLERSTATES.enable == 0)
    {
        TIME_SET(&(CONTROLLERSTATES.stepCycleTimer), PARAMETERS.CONTROLLER_SAMPLETIME);
        return;
    }


    /* read input currents */
    if(CONTROLLERSTATES.currentOverdrive == 1)
    {
        if(SETPOINTS.RELATIVE_TORQUE > 0)
        {
            reference_current = ((((int64_t)(PARAMETERS.OVERDRIVE_FORWARD_LINE_CURRENT)) * SETPOINTS.RELATIVE_TORQUE) / 100) * 1;
        }

        else if(SETPOINTS.RELATIVE_TORQUE < 0)
        {
            reference_current = ((((int64_t)(PARAMETERS.OVERDRIVE_REVERSE_LINE_CURRENT)) * SETPOINTS.RELATIVE_TORQUE) / 100) * -1;
        }

        else
        {
            reference_current = 0;
        }
    }

    else
    {
        if(SETPOINTS.RELATIVE_TORQUE > 0)
        {
            reference_current = ((((int64_t)(PARAMETERS.NOMINAL_FORWARD_LINE_CURRENT)) * SETPOINTS.RELATIVE_TORQUE) / 100) * 1;
        }

        else if(SETPOINTS.RELATIVE_TORQUE < 0)
        {
            reference_current = ((((int64_t)(PARAMETERS.NOMINAL_REVERSE_LINE_CURRENT)) * SETPOINTS.RELATIVE_TORQUE) / 100) * -1;
        }

        else
        {
            reference_current = 0;
        }
    }


    /* calculate the control error */
    CONTROLLERSTATES.error = reference_current - feedback_current;
    CONTROLLERSTATES.ierror = CONTROLLERSTATES.ierror + CONTROLLERSTATES.error;


    /* calculate the controller parts */
    proportional = (int64_t)(PARAMETERS.CONTROLLER_P_VALUE) * CONTROLLERSTATES.error;
    integral = (int64_t)(PARAMETERS.CONTROLLER_I_VALUE) * CONTROLLERSTATES.ierror;


    /* calculate the controller (scaled) output */
    output = (proportional + integral);


    /* check output saturation */
    if(output < 0)
    {
        output = 0;

        if(CONTROLLERSTATES.error < 0)
        {
            CONTROLLERSTATES.ierror = antiwindup_ierror;
        }
    }

    else
    {
        output = output >> 16;

        if(output > (int64_t)(SETPOINTS.TARGET_FREQUENCY))
        {
            output = (int64_t)(SETPOINTS.TARGET_FREQUENCY);

            if(CONTROLLERSTATES.error > 0)
            {
                CONTROLLERSTATES.ierror = antiwindup_ierror;
            }
        }
    }

    MEASUREMENTS.OUTPUT_FREQUENCY = (uint16_t)output;


    /* get values from lookuptable for outputs and send to svpwm ic */
    SVPWM_QUEUE_SET_FREQUENCY((uint16_t)output);
    SVPWM_QUEUE_SET_MAGNITUDE((uint8_t)(PARAMETERS.CONTROLLER_UF_VALUE[((uint16_t)output) >> 6]));
    SVPWM_QUEUE_SEND();


    /* restart sample time timer */
    TIME_SET(&(CONTROLLERSTATES.stepCycleTimer), PARAMETERS.CONTROLLER_SAMPLETIME);
}

uint8_t CONTROLLER_WAIT_CYCLE(void)
{
    int64_t busvoltage = 0;
    int64_t buscurrent = 0;
    int64_t lineAcurrent = 0;
    int64_t lineCcurrent = 0;


    /* read sensor data */
    SENSE_FETCH((uint16_t *)(&busvoltage), (uint16_t *)(&buscurrent), (uint16_t *)(&lineAcurrent), (uint16_t *)(&lineCcurrent));


    /* remove sensor data offset and convert to correct scale */
    busvoltage   = (((busvoltage - 0x0867) *   5) / (4 * 1));
    buscurrent   = (((buscurrent - 0x0867) * 500) / (4 * 4));
    //lineAcurrent = (((lineAcurrent - 0x0870) * 500) / (4 * 4));
    //lineCcurrent = (((lineCcurrent - 0x084D) * 500) / (4 * 4));

    if(busvoltage < 0)
    {
        busvoltage = 0;
    }


    /* apply moving average filter for smoothing */
    MEASUREMENTS.BUS_VOLTAGE = MAVG_UPDATE(&(CONTROLLERSTATES.filtered_busvoltage), (int16_t)(busvoltage));
    MEASUREMENTS.BUS_CURRENT = MAVG_UPDATE(&(CONTROLLERSTATES.filtered_buscurrent), (int16_t)(buscurrent));
    //MEASUREMENTS.LINE_CURRENT = MAVG_UPDATE(&(CONTROLLERSTATES.filtered_linecurrent), (int16_t)(lineAcurrent));
    //MEASUREMENTS.LINE_CURRENT = MAVG_UPDATE(&(CONTROLLERSTATES.filtered_linecurrent), (int16_t)(lineCcurrent));


    /* check controller sample time */
    if(TIME_CHECKEXP(&(CONTROLLERSTATES.stepCycleTimer)))
    {
        /* time to next controller step expired */
        return 0;
    }

    /* another wait cycle is needed */
    return 1;
}
