/*
 * volt_divider.c
 *
 *  Created on: Apr 23, 2024
 *      Author: El Amrani
 */

#include "volt_divider.h"


#define VOLTAGE_DIVIDER_R1 70000 // 70K ohm resistor
#define VOLTAGE_DIVIDER_R2 10000 // 10K ohm resistor
#define VOLTAGE_DIVIDER_R3 90000 // 90K ohm resistor


float readMainVoltage_1(uint32_t adcBase, ADC_SOCNumber socNumber, ADC_IntNumber adcInterruptNumber, uint32_t adcResultBase) {
    // Calculate the voltage across the 10K ohm resistor
    float voltage_out = calculateInputVoltage(adcBase, socNumber, adcInterruptNumber, adcResultBase);
    // Calculate the main voltage
    float voltage_main = voltage_out * ((VOLTAGE_DIVIDER_R1 + VOLTAGE_DIVIDER_R2) / VOLTAGE_DIVIDER_R2);
    return voltage_main;
}

float readMainVoltage_2(uint32_t adcBase, ADC_SOCNumber socNumber, ADC_IntNumber adcInterruptNumber, uint32_t adcResultBase) {
    // Calculate the voltage across the 10K ohm resistor
    float voltage_out = calculateInputVoltage(adcBase, socNumber, adcInterruptNumber, adcResultBase);
    // Calculate the main voltage
    float voltage_main = voltage_out * ((VOLTAGE_DIVIDER_R3 + VOLTAGE_DIVIDER_R2) / VOLTAGE_DIVIDER_R2);
    return voltage_main;
}
