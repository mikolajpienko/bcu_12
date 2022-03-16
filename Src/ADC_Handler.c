/*
 * ADC_Handler.c
 *
 *  Created on: Sep 28, 2021
 *      Author: Micza
 */
#include "ADC_Handler.h"
#include "main.h"
#include "math.h"
#include "string.h"

float VC1 = 0, VC2 = 0, VC3 = 0;
int8_t TEMP1 = 0, TEMP2 = 0, TEMP3 = 0;
uint16_t buf[7];
static uint16_t V1_buffer[VOLT_BUFFER_SIZE];
static uint16_t V2_buffer[VOLT_BUFFER_SIZE];
static uint16_t V3_buffer[VOLT_BUFFER_SIZE];
uint32_t temptick = 0;
float CURRENT = 0;
float Rt = 0;
void ADC_Handler_Init()
{
	  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)buf, 7);
}
float CalcTemp(uint16_t ADC_Value)
{
	Rt = NTC_PULLUP * ((4096/(float)(ADC_Value)));
	return -(298.15*NTC_BETA/(298.15*logf(Rt/NTC_R)+NTC_BETA)-273.15);
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	memcpy(ADC_Values, buf, sizeof(ADC_Values));
	if(HAL_GetTick() - temptick >=1)
	{
		newValuesEvent();
		temptick = HAL_GetTick();
	}
}
void newValuesEvent()
{
	for(uint8_t i = VOLT_BUFFER_SIZE-1; i>0;i--)
	{
		V1_buffer[i] = V1_buffer[i-1];
		V2_buffer[i] = V2_buffer[i-1];
		V3_buffer[i] = V3_buffer[i-1];
	}
	V1_buffer[0] = ADC_Values[0];
	V2_buffer[0] = ADC_Values[1];
	V3_buffer[0] = ADC_Values[2];

}
float mean(uint16_t* array)
{
	uint32_t mean = 0;
	for(uint8_t i = 0; i<VOLT_BUFFER_SIZE;i++)
	{
		mean += array[i];
	}
	return (float)(mean/VOLT_BUFFER_SIZE);
}
float ADC_Handler_GetVC1()
{

	return (((mean(V1_buffer)*3.3)/4096)*VC1_MLTP + VC1_OFFSET)-VC2-VC3;
}
float ADC_Handler_GetVC2()
{
	VC2 = (((mean(V2_buffer)*3.3)/4096)*VC2_MLTP + VC2_OFFSET)-VC3;
	return VC2;
}
float ADC_Handler_GetVC3()
{
	VC3 = ((mean(V3_buffer)*3.3)/4096)*VC3_MLTP + VC3_OFFSET;
	return VC3;
}
float ADC_Handler_GetTemp1()
{
	return CalcTemp(ADC_Values[3]);
}
float ADC_Handler_GetTemp2()
{
	return CalcTemp(ADC_Values[4]);
}
float ADC_Handler_GetTemp3()
{
	return CalcTemp(ADC_Values[5]);
}

