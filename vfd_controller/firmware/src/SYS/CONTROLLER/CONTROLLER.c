#include "CONTROLLER.h"
#include "../REGISTRY/REGISTRY.h"
#include "../../RES/SVPWM/SVPWM.h"
#include "../../RES/SENSE/SENSE.h"

static struct CONTROLLERSTATES_s
{
    int64_t error;
    int64_t ierror;
    uint8_t reversal;
}
CONTROLLERSTATES;

void CONTROLLER_INIT(void)
{
    CONTROLLERSTATES.error = 0;
    CONTROLLERSTATES.ierror = 0;
    CONTROLLERSTATES.reversal = 0;

    SENSE_INIT();
    SVPWM_INIT();
    //SVPWM_START();
    //SVPWM_QUEUE_SET_PWM_FREQUENCY(KHZ_8);
    //SVPWM_QUEUE_SET_MAGNITUDE(0);
    //SVPWM_QUEUE_SET_FREQUENCY(0);
    //SVPWM_QUEUE_SEND();
}

void CONTROLLER_STEP_CYCLE(void)
{
    int64_t reference_current = 0;
    int64_t feedback_current = (int64_t)(MEASUREMENTS.LINE_CURRENT);
    int64_t antiwindup_ierror = CONTROLLERSTATES.ierror;
    int64_t proportional = 0;
    int64_t integral = 0;
    int64_t output = 0;


    /* check setpoint boundaries */
    /* reset overdrives after timeout */

    if((SETPOINTS.ENABLE_FREQUENCY_OVERDRIVE == 0) && (SETPOINTS.TARGET_FREQUENCY > PARAMETERS.NOMINAL_FREQUENCY))
    {
        SETPOINTS.TARGET_FREQUENCY = PARAMETERS.NOMINAL_FREQUENCY;
    }

    else if((SETPOINTS.ENABLE_FREQUENCY_OVERDRIVE == 1) && (SETPOINTS.TARGET_FREQUENCY > PARAMETERS.NOMINAL_FREQUENCY))
    {
        SETPOINTS.TARGET_FREQUENCY = PARAMETERS.OVERDRIVE_FREQUENCY;
    }

    if(SETPOINTS.RELATIVE_TORQUE > 100)
    {
        SETPOINTS.RELATIVE_TORQUE = 100;
    }

    else if(SETPOINTS.RELATIVE_TORQUE < -100)
    {
        SETPOINTS.RELATIVE_TORQUE = -100;
    }


    /* read input currents */
    if(SETPOINTS.ENABLE_CURRENT_OVERDRIVE == 0)
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
            reference_current = ((int64_t)(PARAMETERS.OVERDRIVE_FORWARD_LINE_CURRENT)) * 100;
        }

        else
        {
            reference_current = ((int64_t)(PARAMETERS.OVERDRIVE_REVERSE_LINE_CURRENT)) * 100;
        }
    }

    reference_current = reference_current / SETPOINTS.RELATIVE_TORQUE;


    /* calculate the control error */
    CONTROLLERSTATES.error = reference_current - feedback_current;
    CONTROLLERSTATES.ierror = CONTROLLERSTATES.ierror + CONTROLLERSTATES.error;


    /* calculate the controller terms */
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

    /* check for valid reversal */
    //if((SETPOINTS.RELATIVE_TORQUE <= 0) && (MEASUREMENTS.OUTPUT_FREQUENCY == 0))
    //{
    //
    //}

    SVPWM_QUEUE_SEND();
}

uint8_t CONTROLLER_WAIT_CYCLE(void)
{
    int16_t reference = 0;
    int16_t busvoltage = 0;
    int16_t buscurrent = 0;
    int16_t linecurrent = 0;


    /* read sensor data */
    SENSE_FETCH((uint16_t *)(&reference), (uint16_t *)(&busvoltage), (uint16_t *)(&buscurrent), (uint16_t *)(&linecurrent));

    /* remove sensor data offset and convert to correct scale */
    busvoltage = (busvoltage - reference) * 1;
    buscurrent = (buscurrent - reference) * 1;
    linecurrent = (linecurrent - reference) * 1;

    /* apply low pass filter for smoothing */
    // do something

    if(1) // if cycle time is over continue with the next
    {
        return 1;
    }

    /* another wait cycle is needed */
    return 0;
}
