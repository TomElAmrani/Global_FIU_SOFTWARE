/*
 * interface.c
 *
 *  Created on: Apr 23, 2024
 *      Author: El Amrani
 */
#include "interface.h"

#define VREF 3.3 // Voltage reference in volts

// Function definition
float calculateInputVoltage(uint32_t adcBase, ADC_SOCNumber socNumber, ADC_IntNumber adcInterruptNumber, uint32_t adcResultBase)
{
    int result = 0;

    // Trigger ADC conversion
    ADC_forceSOC(adcBase, socNumber);

    // Wait for ADC conversion to complete
    while (ADC_getInterruptStatus(adcBase, adcInterruptNumber) == false)
    {
    }
    ADC_clearInterruptStatus(adcBase, adcInterruptNumber);

    // Read ADC conversion result
    result = ADC_readResult(adcResultBase, socNumber);

    // Calculate voltage using result and reference voltage
    // ADC resolution is assumed to be 12 bits (4096 levels)
    // Voltage = (ADC_Result / 4096) * VREF
    return ((float)result / 4096.0) * VREF;
}

