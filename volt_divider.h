/*
 * volt_divider.h
 *
 *  Created on: Apr 23, 2024
 *      Author: El Amrani
 */

#ifndef VOLT_DIVIDER_H_
#define VOLT_DIVIDER_H_
#include "interface.h"


float readMainVoltage_1(uint32_t adcBase, ADC_SOCNumber socNumber, ADC_IntNumber adcInterruptNumber, uint32_t adcResultBase);
float readMainVoltage_2(uint32_t adcBase, ADC_SOCNumber socNumber, ADC_IntNumber adcInterruptNumber, uint32_t adcResultBase);


#endif /* VOLT_DIVIDER_H_ */
