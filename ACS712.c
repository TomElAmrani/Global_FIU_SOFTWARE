/*
 * ACS712.c
 *
 *  Created on: Apr 23, 2024
 *      Author: El Amrani
 */

#define SENSIVITY       (float)0.066
#define VOLTAGE_I0      (float)2.5

#include "ACS712.h"

// Convert ADC result to current for ACS712
float ACS712_convertToCurrent(uint32_t adcBase, ADC_SOCNumber socNumber, ADC_IntNumber adcInterruptNumber, uint32_t adcResultBase) {
    float voltage =  calculateInputVoltage(adcBase, socNumber, adcInterruptNumber, adcResultBase);
    float current = (voltage - VOLTAGE_I0) / SENSIVITY; // Convert voltage to current
    return current; // current in A
}
