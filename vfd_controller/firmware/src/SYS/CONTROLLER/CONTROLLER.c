#include "CONTROLLER.h"
#include "./MAVG/MAVG.h"
#include "../REGISTRY/REGISTRY.h"
#include "../../RES/SVPWM/SVPWM.h"
#include "../../RES/SENSE/SENSE.h"
#include "../../RES/TIME/TIME.h"


static struct CONTROLLERSTATES_s
{
    int64_t cerror;
    int64_t ierror;
    TIME_s stepCycleTimer;
    TIME_s overdriveTimer;
    TIME_s enableTimer;
    TIME_s motorUMZTimer;
    TIME_s dcbusUMZTimer;
    MAVG256_s voltageFilterStage1;
    MAVG32_s voltageFilterStage2;
    MAVG256_s currentFilterStage1;
    MAVG32_s currentFilterStage2;
}
CONTROLLERSTATES;


__attribute__((always_inline))
static inline void _controller_check_enable(void)
{
    uint8_t expired = TIME_CHECKEXP(&(CONTROLLERSTATES.enableTimer));

    /* enable flag 0 - normal operation until timer timeout */
    if(SETPOINTS.ENABLE == 0)
    {
        /* check timeout - disable controller if expired */
        if(expired)
        {
            PROCESSVALUES.SYSTEM_STATUS &= ~(SYSTEM_STATUS_ENABLE);
            PROCESSVALUES.SYSTEM_STATUS &= ~(SYSTEM_STATUS_RUN);
            PROCESSVALUES.SYSTEM_STATUS |=  (SYSTEM_STATUS_STOP);
            PROCESSVALUES.MOTOR_FREQUENCY = 0;
            SVPWM_STOP();
            return;
        }
    }

    /* enable flag 1 - normal/startup operation until timer timeout */
    else if(SETPOINTS.ENABLE == 1)
    {
        /* reset enable timer */
        SETPOINTS.ENABLE = 0;
        TIME_SET(&(CONTROLLERSTATES.enableTimer), PARAMETERS.CONTROLLER_ENABLE_TIMEOUT);

        /* check if controller was previously disabled - reactivate it */
        if((PROCESSVALUES.SYSTEM_STATUS & (SYSTEM_STATUS_ENABLE)) == 0)
        {
            PROCESSVALUES.SYSTEM_STATUS |=  (SYSTEM_STATUS_ENABLE);
            PROCESSVALUES.SYSTEM_STATUS &= ~(SYSTEM_STATUS_STOP);
            PROCESSVALUES.SYSTEM_STATUS |=  (SYSTEM_STATUS_RUN);
            CONTROLLERSTATES.cerror = 0;
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
        SETPOINTS.ENABLE = -1;
        PROCESSVALUES.SYSTEM_STATUS &= ~(SYSTEM_STATUS_ENABLE);
        PROCESSVALUES.SYSTEM_STATUS &= ~(SYSTEM_STATUS_RUN);
        PROCESSVALUES.SYSTEM_STATUS |=  (SYSTEM_STATUS_STOP);
        PROCESSVALUES.MOTOR_FREQUENCY = 0;
        SVPWM_STOP();
        return;
    }
}

__attribute__((always_inline))
static inline void _controller_check_overdrive(void)
{
    uint8_t expired = TIME_CHECKEXP(&(CONTROLLERSTATES.overdriveTimer));

    /* reset overdrive, switch to cooldown */
    if((expired) && ((PROCESSVALUES.SYSTEM_STATUS & SYSTEM_STATUS_CURRENT_OVERDRIVE) == 1))
    {
        /* disable overdrive, start cooldown */
        PROCESSVALUES.SYSTEM_STATUS &= ~(SYSTEM_STATUS_CURRENT_OVERDRIVE);
        SETPOINTS.ENABLE_CURRENT_OVERDRIVE = 0;
        TIME_SET(&(CONTROLLERSTATES.overdriveTimer), PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN);
    }

    /* reset cooldown after timeout */
    else if((expired) && ((PROCESSVALUES.SYSTEM_STATUS & SYSTEM_STATUS_CURRENT_OVERDRIVE) == 0))
    {
        /* enable overdrive, start timeout */
        if(SETPOINTS.ENABLE_CURRENT_OVERDRIVE == 1)
        {
            PROCESSVALUES.SYSTEM_STATUS |= (SYSTEM_STATUS_CURRENT_OVERDRIVE);
            TIME_SET(&(CONTROLLERSTATES.overdriveTimer), PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT);
        }
    }
}

__attribute__((always_inline))
static inline void _controller_check_overload(void)
{
    PROCESSVALUES.SYSTEM_STATUS &= ~(SYSTEM_STATUS_DCBUS_UMZ_WARNING);
    PROCESSVALUES.SYSTEM_STATUS &= ~(SYSTEM_STATUS_MOTOR_UMZ_WARNING);

    if((PROCESSVALUES.DCBUS_VOLTAGE < PARAMETERS.CONTROLLER_DCBUS_MAXIMAL_VOLTAGE) && (PROCESSVALUES.DCBUS_VOLTAGE > PARAMETERS.CONTROLLER_DCBUS_MINIMAL_VOLTAGE))
    {
        TIME_SET(&(CONTROLLERSTATES.dcbusUMZTimer), PARAMETERS.CONTROLLER_DCBUS_UMZ_TIMEOUT);
    }

    else
    {
        if(!(TIME_CHECKEXP(&(CONTROLLERSTATES.dcbusUMZTimer))))
        {
            PROCESSVALUES.SYSTEM_STATUS |= (SYSTEM_STATUS_DCBUS_UMZ_WARNING);
        }

        else
        {
            PROCESSVALUES.SYSTEM_STATUS |=  (SYSTEM_STATUS_ERROR);
            PROCESSVALUES.SYSTEM_STATUS |=  (SYSTEM_STATUS_DCBUS_UMZ_ERROR);
        }
    }


    if(((PROCESSVALUES.SYSTEM_STATUS & SYSTEM_STATUS_CURRENT_OVERDRIVE) == 0) && (PROCESSVALUES.MOTOR_CURRENT < PARAMETERS.MOTOR_NOMINAL_FWD_CURRENT) && (PROCESSVALUES.MOTOR_CURRENT > -((int32_t)PARAMETERS.MOTOR_NOMINAL_REV_CURRENT)))
    {
        TIME_SET(&(CONTROLLERSTATES.motorUMZTimer), PARAMETERS.CONTROLLER_NOMINAL_UMZ_TIMEOUT);
    }

    else if(((PROCESSVALUES.SYSTEM_STATUS & SYSTEM_STATUS_CURRENT_OVERDRIVE) == 1) && (PROCESSVALUES.MOTOR_CURRENT < PARAMETERS.MOTOR_OVERDRIVE_FWD_CURRENT) && (PROCESSVALUES.MOTOR_CURRENT > -((int32_t)PARAMETERS.MOTOR_OVERDRIVE_REV_CURRENT)))
    {
        TIME_SET(&(CONTROLLERSTATES.motorUMZTimer), PARAMETERS.CONTROLLER_OVERDRIVE_UMZ_TIMEOUT);
    }

    else
    {
        if(!(TIME_CHECKEXP(&(CONTROLLERSTATES.motorUMZTimer))))
        {
            PROCESSVALUES.SYSTEM_STATUS |= (SYSTEM_STATUS_MOTOR_UMZ_WARNING);
        }

        else
        {
            PROCESSVALUES.SYSTEM_STATUS |=  (SYSTEM_STATUS_ERROR);
            PROCESSVALUES.SYSTEM_STATUS |=  (SYSTEM_STATUS_MOTOR_UMZ_ERROR);
        }
    }
}


void CONTROLLER_INIT(void)
{
    /* initialize controller states */
    CONTROLLERSTATES.cerror = 0;
    CONTROLLERSTATES.ierror = 0;
    PROCESSVALUES.SYSTEM_STATUS &= ~(SYSTEM_STATUS_ENABLE);
    PROCESSVALUES.SYSTEM_STATUS &= ~(SYSTEM_STATUS_CURRENT_OVERDRIVE);

    /* initialize timing intervals */
    TIME_INIT();
    TIME_SET(&(CONTROLLERSTATES.stepCycleTimer), PARAMETERS.CONTROLLER_SAMPLETIME);
    TIME_SET(&(CONTROLLERSTATES.overdriveTimer), PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT);
    TIME_SET(&(CONTROLLERSTATES.motorUMZTimer), PARAMETERS.CONTROLLER_NOMINAL_UMZ_TIMEOUT);
    TIME_SET(&(CONTROLLERSTATES.dcbusUMZTimer), PARAMETERS.CONTROLLER_DCBUS_UMZ_TIMEOUT);
    TIME_SET(&(CONTROLLERSTATES.enableTimer), 1);

    /* initialize filter */
    MAVG256_INIT(&(CONTROLLERSTATES.voltageFilterStage1));
    MAVG32_INIT(&(CONTROLLERSTATES.voltageFilterStage2));
    MAVG256_INIT(&(CONTROLLERSTATES.currentFilterStage1));
    MAVG32_INIT(&(CONTROLLERSTATES.currentFilterStage2));

    /* initialize measuring */
    SENSE_INIT();

    /* initialize svpwm subsystem */
    SVPWM_INIT();
    SVPWM_STOP();
    SVPWM_QUEUE_SET_PWM_FREQUENCY(PARAMETERS.CONTROLLER_PWM_FREQUENCY);
    SVPWM_QUEUE_SET_DEADTIME_PRESCALER(PARAMETERS.CONTROLLER_PWM_DEADTIME_PRESCALING);
    SVPWM_QUEUE_SET_DEADTIME_TIMINGS(((PARAMETERS.CONTROLLER_PWM_FREQUENCY >> 8) & 0x0F), ((PARAMETERS.CONTROLLER_PWM_FREQUENCY >> 0) & 0x0F));
    SVPWM_QUEUE_SET_MAGNITUDE(0);
    SVPWM_QUEUE_SET_FREQUENCY(0);
    SVPWM_QUEUE_SEND();
}

void CONTROLLER_STEP_CYCLE(void)
{
    /* declare some working variables */
    int64_t reference_current = 0;
    int64_t feedback_current = 0;
    int64_t feedback_voltage = 0;
    int64_t antiwindup_ierror = CONTROLLERSTATES.ierror;
    int64_t proportional = 0;
    int64_t integral = 0;
    int64_t output = 0;

    /* move data to process register  */
    PROCESSVALUES.DCBUS_VOLTAGE = MAVG32_UPDATE(&(CONTROLLERSTATES.voltageFilterStage2), MAVG256_GET(&(CONTROLLERSTATES.voltageFilterStage1)));// + PARAMETERS.CONTROLLER_DCBUS_VOLTAGE_OFFSET;
    PROCESSVALUES.DCBUS_CURRENT = MAVG32_UPDATE(&(CONTROLLERSTATES.currentFilterStage2), MAVG256_GET(&(CONTROLLERSTATES.currentFilterStage1)));// + PARAMETERS.CONTROLLER_DCBUS_CURRENT_OFFSET;
    feedback_current = (int64_t)(PROCESSVALUES.DCBUS_CURRENT);
    feedback_voltage = (int64_t)(PROCESSVALUES.DCBUS_VOLTAGE);

    /* calculate depending measurement values */
    /* approximation of sqrt(2) by (1448/1024) and 1/sqrt(2) by (1448/2048) */
    feedback_voltage = ((((feedback_voltage * 1448) / (2048 * 2)) * ((PARAMETERS.CONTROLLER_UF_VALUE[(PROCESSVALUES.MOTOR_FREQUENCY) / 32]))) >> 8);
    feedback_current = ((feedback_current * 1448) / 2048);

    PROCESSVALUES.MOTOR_VOLTAGE = (uint16_t)feedback_voltage;
    PROCESSVALUES.MOTOR_CURRENT = (int16_t)feedback_current;

    /* check for enable and overdrive */
    _controller_check_enable();
    _controller_check_overload();
    _controller_check_overdrive();

    /* check for system error */
    if((PROCESSVALUES.SYSTEM_STATUS & SYSTEM_STATUS_ERROR) == 1)
    {
        PROCESSVALUES.SYSTEM_STATUS &= ~(SYSTEM_STATUS_ENABLE);
        PROCESSVALUES.SYSTEM_STATUS &= ~(SYSTEM_STATUS_RUN);
        PROCESSVALUES.SYSTEM_STATUS &= ~(SYSTEM_STATUS_STOP);
        PROCESSVALUES.MOTOR_FREQUENCY = 0;
        SVPWM_STOP();
        TIME_SET(&(CONTROLLERSTATES.stepCycleTimer), PARAMETERS.CONTROLLER_SAMPLETIME);
        return;
    }

    /* check if controller calculation is necessary */
    if((PROCESSVALUES.SYSTEM_STATUS & SYSTEM_STATUS_ENABLE) == 0)
    {
        TIME_SET(&(CONTROLLERSTATES.stepCycleTimer), PARAMETERS.CONTROLLER_SAMPLETIME);
        return;
    }

    /* read input currents */
    if(((PROCESSVALUES.MOTOR_FREQUENCY > SETPOINTS.TARGET_FREQUENCY) && (SETPOINTS.TARGET_TORQUE > 0)) || (SETPOINTS.TARGET_TORQUE == 0))
    {
        reference_current = 0;
    }

    else
    {
        if((PROCESSVALUES.SYSTEM_STATUS & SYSTEM_STATUS_CURRENT_OVERDRIVE) == 1)
        {
            if(SETPOINTS.TARGET_TORQUE > 0)
            {
                reference_current = ((((int64_t)(PARAMETERS.MOTOR_OVERDRIVE_FWD_CURRENT)) * SETPOINTS.TARGET_TORQUE) / 100) * 1;
            }

            else if(SETPOINTS.TARGET_TORQUE < 0)
            {
                reference_current = ((((int64_t)(PARAMETERS.MOTOR_OVERDRIVE_REV_CURRENT)) * SETPOINTS.TARGET_TORQUE) / 100) * -1;
            }
        }

        else
        {
            if(SETPOINTS.TARGET_TORQUE > 0)
            {
                reference_current = ((((int64_t)(PARAMETERS.MOTOR_NOMINAL_FWD_CURRENT)) * SETPOINTS.TARGET_TORQUE) / 100) * 1;
            }

            else if(SETPOINTS.TARGET_TORQUE < 0)
            {
                reference_current = ((((int64_t)(PARAMETERS.MOTOR_NOMINAL_REV_CURRENT)) * SETPOINTS.TARGET_TORQUE) / 100) * -1;
            }
        }
    }


    /* calculate the control error */
    CONTROLLERSTATES.cerror = reference_current - feedback_current;
    CONTROLLERSTATES.ierror = CONTROLLERSTATES.ierror + CONTROLLERSTATES.cerror;


    /* calculate the controller parts */
    proportional = (int64_t)(PARAMETERS.CONTROLLER_PROPORTIONAL_FACTOR) * CONTROLLERSTATES.cerror;
    integral = (int64_t)(PARAMETERS.CONTROLLER_INTEGRAL_FACTOR) * CONTROLLERSTATES.ierror;


    /* calculate the controller (scaled) output */
    output = (proportional + integral);


    /* check output saturation */
    if(output < 0)
    {
        output = 0;

        if(CONTROLLERSTATES.cerror < 0)
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

            if(CONTROLLERSTATES.cerror > 0)
            {
                CONTROLLERSTATES.ierror = antiwindup_ierror;
            }
        }
    }

    PROCESSVALUES.MOTOR_FREQUENCY = (uint16_t)output;


    /* get values from lookuptable for outputs and send to svpwm ic */
    SVPWM_QUEUE_SET_FREQUENCY((uint16_t)output);
    SVPWM_QUEUE_SET_MAGNITUDE((uint8_t)(PARAMETERS.CONTROLLER_UF_VALUE[((uint16_t)output) / 32]));
    SVPWM_QUEUE_SEND();


    /* restart sample time timer */
    TIME_SET(&(CONTROLLERSTATES.stepCycleTimer), PARAMETERS.CONTROLLER_SAMPLETIME);
}

uint8_t CONTROLLER_WAIT_CYCLE(void)
{
    int64_t busvoltage = 0;
    int64_t buscurrent = 0;
    int64_t noise1 = 0;
    int64_t noise2 = 0;

    /* read sensor data */
    SENSE_FETCH((uint16_t *)(&busvoltage), (uint16_t *)(&noise1), (uint16_t *)(&noise2), (uint16_t *)(&buscurrent));

    /* remove sensor data offset and convert to correct scale */
    noise1 = (noise1 + noise2) / 2;
    busvoltage = (((busvoltage - noise1) *   5) / (4 * 1));
    buscurrent = (((buscurrent - noise1) * 500) / (4 * 4));
    //busvoltage = ((busvoltage - 2315) * 41) / 32;
    //buscurrent = (((buscurrent - noise1) * 500) / (4 * 4));


    /* put data in stage 1 buffer */
    MAVG256_UPDATE(&(CONTROLLERSTATES.voltageFilterStage1), (int16_t)(busvoltage));
    MAVG256_UPDATE(&(CONTROLLERSTATES.currentFilterStage1), (int16_t)(buscurrent));

    /* check controller sample time */
    if(TIME_CHECKEXP(&(CONTROLLERSTATES.stepCycleTimer)))
    {
        /* time to next controller step expired */
        return 0;
    }

    /* another wait cycle is needed */
    return 1;
}
