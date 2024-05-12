/*
 * interface.h
 *
 *  Created on: Apr 23, 2024
 *      Author: El Amrani
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_
#include "driverlib.h"


// Function declaration
float calculateInputVoltage(uint32_t adcBase, ADC_SOCNumber socNumber, ADC_IntNumber adcInterruptNumber, uint32_t adcResultBase);



#endif /* INTERFACE_H_ */
