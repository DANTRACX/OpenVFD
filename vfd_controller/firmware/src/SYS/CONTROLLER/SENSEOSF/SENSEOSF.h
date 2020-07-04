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
 * Module name : SENSEOSF
 * Contents	   : SENSEOSF.h, SENSEOSF.c
 * Contributors: Christoph Klie
 *
 * This SENSEOSF module implements an oversampling and filtering algorithm
 * for the measurements provided by the adc from the sense module to
 * eliminate noise.
 * Two sets of functions are impelmented for voltage and current measurements
 * with different resolution.
 * **************************************************************************
 */

#ifndef __SENSEOSF_H__
#define __SENSEOSF_H__


/* Header files - Include the global headers used by this module */
#include <stdint.h>


/* SENSEOSF datatype - Used for storing filter data */
typedef struct SENSEOSF_s
{
        uint16_t    oversampleCounter;
        uint64_t    sampleSum;
        uint16_t    oversampledResult;
        uint8_t     medianSampleCounter;
        uint16_t    medianBuffer[5];
        uint16_t    median;
}
SENSEOSF_s;


/* Forward declarations - Make functions accessible */
/**
 * @brief Function for initializing the data structure
 *
 * This function initializes the variable for the oversampling and filter
 *
 */
extern void SENSEOSF_INIT(SENSEOSF_s *ptr);


extern uint16_t SENSEOSF_U_GET(SENSEOSF_s *ptr);
extern uint16_t SENSEOSF_U_UPDATE(SENSEOSF_s *ptr, uint16_t value);
extern uint16_t SENSEOSF_I_GET(SENSEOSF_s *ptr);
extern uint16_t SENSEOSF_I_UPDATE(SENSEOSF_s *ptr, uint16_t value);

#endif
