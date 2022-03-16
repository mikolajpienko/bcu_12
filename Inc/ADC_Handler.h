/*
 * ADC_Handler.h
 *
 *  Created on: Sep 28, 2021
 *      Author: Micza
 */

#ifndef INC_ADC_HANDLER_H_
#define INC_ADC_HANDLER_H_

#include "stdio.h"

#define VOLT_BUFFER_SIZE 100

#define VC1_MLTP 4.75
#define VC1_OFFSET -0.1

#define VC2_MLTP 3.25
#define VC2_OFFSET -0.1

#define VC3_MLTP 1.55555556
#define VC3_OFFSET 0

#define NTC_PULLUP 100000.0f
#define NTC_R 100000.0f
#define NTC_BETA 4250.0f

#define TEMP1_MLTP 1
#define TEMP1_OFFSET 0

#define TEMP2_MLTP 1
#define TEMP2_OFFSET 0

#define TEMP3_MLTP 1
#define TEMP3_OFFSET 0


uint16_t ADC_Values[7];

void ADC_Handler_Init(void);
void newValuesEvent(void);
float ADC_Handler_GetVC1(void);
float ADC_Handler_GetVC2(void);
float ADC_Handler_GetVC3(void);
float ADC_Handler_GetTemp1(void);
float ADC_Handler_GetTemp2(void);
float ADC_Handler_GetTemp3(void);



#endif /* INC_ADC_HANDLER_H_ */
