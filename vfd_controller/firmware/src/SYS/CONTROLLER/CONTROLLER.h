/*
 * Copyright 2020
 *
 * Christoph Klie
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* **************************************************************************
 * Module name : CONTROLLER
 * Contents	   : CONTROLLER.h, CONTROLLER.c
 * Contributors: Christoph Klie
 *
 * This CONTROLLER module implements the control loop for constant current
 * and therefor constant output torque. Additionally all safty aspects of the
 * devices are impemented. This module represents the main interface between
 * all software components and the actual hardware.
 * The INIT function initializes the controller with initial values for
 * controller, filter, timers and the interface to the svpwm subsystem.
 * The main STEP_CYLCE function calulates the voltage and frequency output
 * from all given system parameters and input values.
 * The WAIT_CYCLE function ensures a constant controller sample time and uses
 * the time to get voltage and current measurements. The WAIT_CYCLE function
 * should be called in a loop until its return value becomes 0 (time step
 * expired), following a STEP_CYCLES call for calculating the output values.
 * **************************************************************************
 */

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__


/* Header files - Include the global headers used by this module */
#include <stdint.h>


/* Forward declarations - Make functions accessible */
/**
 * @brief Function for initializing the software controller
 *
 * This function initializes the variables for the controller, timers, filter
 * and the interace to the svpwm subsystem.
 *
 */
extern  void CONTROLLER_INIT(void);

/**
 * @brief Function for calculating controller outputs for one timestep
 *
 * This function calculates the output values for the svpwm subsystem based
 * on system measurements. Additionally all safty aspects are implemented in
 * this function.
 *
 */
extern  void CONTROLLER_STEP_CYCLE(void);

/**
 * @brief Function for waiting for expired time step
 *
 * This function checks if the controller's timestep has expired or if it is
 * necessary to wait. On each function call, the measurements form the
 * hardware are scaled an put into the filter.
 *
 * @return Returns Zero if timestep has expired and one if waiting is needed
 */
extern  uint8_t CONTROLLER_WAIT_CYCLE(void);

/* Example Code - Shows a minimal working example */
/*
 * [MAIN FUNCTION]
 * ...
 *
 * while(1)
 * {
 *    CONTROLLER_STEP_CYCLE();
 *
 *    do
 *    {
 *            wdt_reset();
 *    }
 *    while(CONTROLLER_WAIT_CYCLE());
 * }
 *
 */

#endif
