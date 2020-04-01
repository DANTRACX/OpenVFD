#include "CONTROLLER.h"
#include "./MAVG/MAVG.h"
#include "../REGISTRY/REGISTRY.h"
#include "../../RES/SVPWM/SVPWM.h"
#include "../../RES/SENSE/SENSE.h"
#include "../../RES/TIME/TIME.h"

#include "../../RES/RS232/RS232.h"

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
    RS232_INIT();
}

void CONTROLLER_STEP_CYCLE(void)
{
    int64_t reference_current = 0;
    int64_t feedback_current = (int64_t)(MEASUREMENTS.LINE_CURRENT);
    int64_t antiwindup_ierror = CONTROLLERSTATES.ierror;
    int64_t proportional = 0;
    int64_t integral = 0;
    int64_t output = 0;
    uint8_t expired = 0;


    /* reset overdrive after timeout */
    expired = TIME_CHECKEXP(&(CONTROLLERSTATES.overdriveTimer));

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


    /* reset enable after timeout */
    expired = TIME_CHECKEXP(&(CONTROLLERSTATES.enableTimer));

    if(SETPOINTS.TIMED_ENABLE == 0)
    {
        /* normal operation - check if timeout expired */
        if(expired)
        {
            CONTROLLERSTATES.enable = 0;
            MEASUREMENTS.OUTPUT_FREQUENCY = 0;
            SVPWM_STOP();
            TIME_SET(&(CONTROLLERSTATES.stepCycleTimer), PARAMETERS.CONTROLLER_SAMPLETIME);
            return;
        }
    }

    else if(SETPOINTS.TIMED_ENABLE == 1)
    {
        /* reactivate controller */
        SETPOINTS.TIMED_ENABLE = 0;
        TIME_SET(&(CONTROLLERSTATES.enableTimer), PARAMETERS.ENABLE_TIMEOUT);

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

        else if((CONTROLLERSTATES.enable == 1) && (expired))
        {
            CONTROLLERSTATES.enable = 0;
            MEASUREMENTS.OUTPUT_FREQUENCY = 0;
            SVPWM_STOP();
            TIME_SET(&(CONTROLLERSTATES.stepCycleTimer), PARAMETERS.CONTROLLER_SAMPLETIME);
            return;
        }
    }

    else
    {
        /* emergency stop */
        SETPOINTS.TIMED_ENABLE = -1;
        CONTROLLERSTATES.enable = 0;
        MEASUREMENTS.OUTPUT_FREQUENCY = 0;
        SVPWM_STOP();
        TIME_SET(&(CONTROLLERSTATES.stepCycleTimer), PARAMETERS.CONTROLLER_SAMPLETIME);
        return;
    }


    /* check setpoint boundaries */
    if(SETPOINTS.RELATIVE_TORQUE > 100)
    {
        SETPOINTS.RELATIVE_TORQUE = 100;
    }

    else if(SETPOINTS.RELATIVE_TORQUE < -100)
    {
        SETPOINTS.RELATIVE_TORQUE = -100;
    }

    if((SETPOINTS.ENABLE_FREQUENCY_OVERDRIVE == 0) && (SETPOINTS.TARGET_FREQUENCY > PARAMETERS.NOMINAL_FREQUENCY))
    {
        SETPOINTS.TARGET_FREQUENCY = PARAMETERS.NOMINAL_FREQUENCY;
    }

    else if((SETPOINTS.ENABLE_FREQUENCY_OVERDRIVE == 1) && (SETPOINTS.TARGET_FREQUENCY > PARAMETERS.NOMINAL_FREQUENCY))
    {
        SETPOINTS.TARGET_FREQUENCY = PARAMETERS.OVERDRIVE_FREQUENCY;
    }

    /* read input currents */
    if(CONTROLLERSTATES.currentOverdrive == 1)
    {
        if(SETPOINTS.RELATIVE_TORQUE > 0)
        {
            reference_current = ((int64_t)(PARAMETERS.OVERDRIVE_FORWARD_LINE_CURRENT)) * 100;
        }

        else
        {
            reference_current = ((int64_t)(PARAMETERS.OVERDRIVE_REVERSE_LINE_CURRENT)) * 100;
        }
    }

    else
    {
        if(SETPOINTS.RELATIVE_TORQUE > 0)
        {
            reference_current = ((int64_t)(PARAMETERS.NOMINAL_FORWARD_LINE_CURRENT)) * 100;
        }

        else
        {
            reference_current = ((int64_t)(PARAMETERS.NOMINAL_REVERSE_LINE_CURRENT)) * 100;
        }
    }

    if(SETPOINTS.RELATIVE_TORQUE > 0)
    {
        reference_current = reference_current / SETPOINTS.RELATIVE_TORQUE;
    }

    else
    {
        reference_current = 0;
    }

    /* TODO ADD SCALING TO INPUT AND OUTPUTS !!!! */
    /* calculate the control error */
    CONTROLLERSTATES.error = reference_current - feedback_current;
    CONTROLLERSTATES.ierror = CONTROLLERSTATES.ierror + CONTROLLERSTATES.error;


    /* calculate the controller parts */
    proportional = (int64_t)(PARAMETERS.CONTROLLER_P_VALUE) * CONTROLLERSTATES.error;
    integral = (int64_t)(PARAMETERS.CONTROLLER_I_VALUE) * CONTROLLERSTATES.ierror;


    /* calculate the controller (scaled) output */
    output = (proportional + integral);


    /* check output saturation */
    if(output <= 0)
    {
        output = 0;

        if(CONTROLLERSTATES.error < 0)
        {
            CONTROLLERSTATES.ierror = antiwindup_ierror;
        }
    }

    else
    {
        output = output >> 8; /* output scaling */

        if(output > (int64_t)(SETPOINTS.TARGET_FREQUENCY))
        {
            output = (int64_t)(SETPOINTS.TARGET_FREQUENCY);

            if(CONTROLLERSTATES.error > 0)
            {
                CONTROLLERSTATES.ierror = antiwindup_ierror;
            }
        }
    }

    MEASUREMENTS.OUTPUT_FREQUENCY = output;


    /* get values from lookuptable for outputs */
    output = (output & 0x000000000000FFFF);
    SVPWM_QUEUE_SET_FREQUENCY((uint16_t)(output));
    SVPWM_QUEUE_SET_MAGNITUDE((uint8_t)(PARAMETERS.CONTROLLER_UF_VALUE[(uint16_t)(output >> 6)]));


    /* send command to svpwm ic and restart sample time timer */
    SVPWM_QUEUE_SEND();
    TIME_SET(&(CONTROLLERSTATES.stepCycleTimer), PARAMETERS.CONTROLLER_SAMPLETIME);
}

uint8_t CONTROLLER_WAIT_CYCLE(void)
{
    static uint32_t counter = 0;
    char output[4];
    int32_t busvoltage = 0;
    int32_t buscurrent = 0;
    int32_t lineAcurrent = 0;
    int32_t lineCcurrent = 0;


    /* read sensor data */
    SENSE_FETCH((uint16_t *)(&busvoltage), (uint16_t *)(&buscurrent), (uint16_t *)(&lineAcurrent), (uint16_t *)(&lineCcurrent));


    /* remove sensor data offset and convert to correct scale */
    busvoltage   = (((busvoltage   - 0x084D) *   5) / (4 * 1));
    buscurrent   = (((buscurrent   - 0x084D) * 500) / (4 * 4));
    //lineAcurrent = (((lineAcurrent - 0x0870) * 500) / (4 * 4));
    //lineCcurrent = (((lineCcurrent - 0x084D) * 500) / (4 * 4));


    /* apply moving average filter for smoothing */
    MEASUREMENTS.BUS_VOLTAGE = MAVG_UPDATE(&(CONTROLLERSTATES.filtered_busvoltage), (int16_t)(busvoltage));
    MEASUREMENTS.BUS_CURRENT = MAVG_UPDATE(&(CONTROLLERSTATES.filtered_buscurrent), (int16_t)(buscurrent));


    /* calculate depending measurement values */
    MEASUREMENTS.LINE_CURRENT = MEASUREMENTS.BUS_CURRENT;


    /* check controller sample time */
    if(TIME_CHECKEXP(&(CONTROLLERSTATES.stepCycleTimer)))
    {
        /* time to next controller step expired */
        //output[0] = (char)(0xFF);
        //output[1] = (char)(counter >> 16);
        //output[2] = (char)(counter >> 8);
        //output[3] = (char)(counter >> 0);
        output[0] = (char)(0xFF);
        output[1] = (char)(MEASUREMENTS.OUTPUT_FREQUENCY >> 8);
        output[2] = (char)(MEASUREMENTS.OUTPUT_FREQUENCY >> 0);
        output[3] = (char)0xFF;
        RS232_SEND(output, 4);
        counter = 0;
        return 0;
    }

    counter++;
    /* another wait cycle is needed */
    return 1;
}
