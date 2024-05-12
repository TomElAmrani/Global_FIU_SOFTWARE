/*
 * ACS712.h
 *
 *  Created on: Apr 23, 2024
 *      Author: El Amrani
 */

#ifndef ACS712_H_
#define ACS712_H_
#include "interface.h"


float ACS712_convertToCurrent(uint32_t adcBase, ADC_SOCNumber socNumber, ADC_IntNumber adcInterruptNumber, uint32_t adcResultBase);



#endif /* ACS712_H_ */
