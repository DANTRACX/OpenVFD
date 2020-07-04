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


/* Header files - Includes local used header files */
#include "SENSEOSF.h"


/* Sorting function - Sorts two elements by their size */
__attribute__((always_inline))
static inline void networkSort(uint16_t *lowValue, uint16_t *highValue)
{
    uint16_t temp;

    if(*lowValue > *highValue)
    {
        temp = *highValue;
        *highValue = *lowValue;
        *lowValue = temp;
    }
}


/* Initialization function - Initialize the data structure */
void SENSEOSF_INIT(SENSEOSF_s *ptr)
{
    ptr->oversampleCounter = 0;
    ptr->sampleSum = 0;
    ptr->oversampledResult = 0;
    ptr->medianSampleCounter = 0;
    ptr->median = 0;
    ptr->medianBuffer[0] = 0;
    ptr->medianBuffer[1] = 0;
    ptr->medianBuffer[2] = 0;
    ptr->medianBuffer[3] = 0;
    ptr->medianBuffer[4] = 0;
}


/* Fetch function - Get the current result of the voltage filter output */
uint16_t SENSEOSF_U_GET(SENSEOSF_s *ptr)
{
    /* return current result of voltage filter output */
    return (uint16_t)(ptr->median);
}


/* Update function - Add a new element to the voltage filter */
uint16_t SENSEOSF_U_UPDATE(SENSEOSF_s *ptr, uint16_t value)
{
    /* add new datapoint to the sum , increment counter */
    ptr->sampleSum = ptr->sampleSum + value;
    ptr->oversampleCounter = ptr->oversampleCounter + 1;

    /* counter reached top of filter elements, calculate oversampled value */
    if(ptr->oversampleCounter == 64)
    {
        ptr->oversampledResult = (uint16_t)(ptr->sampleSum >> 3);
        ptr->oversampleCounter = 0;
        ptr->sampleSum = 0;

        ptr->medianBuffer[ptr->medianSampleCounter] = ptr->oversampledResult;
        ptr->medianSampleCounter = ptr->medianSampleCounter + 1;

        if(ptr->medianSampleCounter == 5)
        {
            /* sorting network */
            networkSort(&(ptr->medianBuffer[0]), &(ptr->medianBuffer[4]));
            networkSort(&(ptr->medianBuffer[1]), &(ptr->medianBuffer[4]));
            networkSort(&(ptr->medianBuffer[2]), &(ptr->medianBuffer[4]));
            networkSort(&(ptr->medianBuffer[3]), &(ptr->medianBuffer[4]));
            networkSort(&(ptr->medianBuffer[0]), &(ptr->medianBuffer[2]));
            networkSort(&(ptr->medianBuffer[1]), &(ptr->medianBuffer[3]));
            networkSort(&(ptr->medianBuffer[0]), &(ptr->medianBuffer[1]));
            networkSort(&(ptr->medianBuffer[2]), &(ptr->medianBuffer[3]));
            networkSort(&(ptr->medianBuffer[1]), &(ptr->medianBuffer[2]));

            ptr->medianSampleCounter = 0;
            ptr->median = ptr->medianBuffer[2];
        }
    }

    /* return current result of voltage filter output */
    return (uint16_t)(ptr->median);
}



/* Fetch function - Get the current result of the voltage filter output */
uint16_t SENSEOSF_I_GET(SENSEOSF_s *ptr)
{
    /* return current result of voltage filter output */
    return (uint16_t)(ptr->median);
}


/* Update function - Add a new element to the voltage filter */
uint16_t SENSEOSF_I_UPDATE(SENSEOSF_s *ptr, uint16_t value)
{
    /* add new datapoint to the sum , increment counter */
    ptr->sampleSum = ptr->sampleSum + value;
    ptr->oversampleCounter = ptr->oversampleCounter + 1;

    /* counter reached top of filter elements, calculate oversampled value */
    if(ptr->oversampleCounter == 64)
    {
        ptr->oversampledResult = (uint16_t)(ptr->sampleSum >> 3);
        ptr->oversampleCounter = 0;
        ptr->sampleSum = 0;

        ptr->medianBuffer[ptr->medianSampleCounter] = ptr->oversampledResult;
        ptr->medianSampleCounter = ptr->medianSampleCounter + 1;

        if(ptr->medianSampleCounter == 5)
        {
            /* sorting network */
            networkSort(&(ptr->medianBuffer[0]), &(ptr->medianBuffer[4]));
            networkSort(&(ptr->medianBuffer[1]), &(ptr->medianBuffer[4]));
            networkSort(&(ptr->medianBuffer[2]), &(ptr->medianBuffer[4]));
            networkSort(&(ptr->medianBuffer[3]), &(ptr->medianBuffer[4]));
            networkSort(&(ptr->medianBuffer[0]), &(ptr->medianBuffer[2]));
            networkSort(&(ptr->medianBuffer[1]), &(ptr->medianBuffer[3]));
            networkSort(&(ptr->medianBuffer[0]), &(ptr->medianBuffer[1]));
            networkSort(&(ptr->medianBuffer[2]), &(ptr->medianBuffer[3]));
            networkSort(&(ptr->medianBuffer[1]), &(ptr->medianBuffer[2]));

            ptr->medianSampleCounter = 0;
            ptr->median = ptr->medianBuffer[2];
        }
    }

    /* return current result of voltage filter output */
    return (uint16_t)(ptr->median);
}
