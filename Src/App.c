/*
 * App.c
 *
 *  Created on: Sep 26, 2021
 *      Author: Micza
 */

#define BOOT_DELAY 1000

#include "App.h"
#include "main.h"
#include "ADC_Handler.h"
#include "CAN_Handler.h"
#include "math.h"
void ApplicationInit()
{
	HAL_GPIO_WritePin(DSCH_1_GPIO_Port, DSCH_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DSCH_2_GPIO_Port, DSCH_2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DSCH_3_GPIO_Port, DSCH_3_Pin, GPIO_PIN_RESET);

	ADC_Handler_Init();
	CAN_Handler_Init();

	State = Init;
	while(1)
	{
		tick = HAL_GetTick();
		if(tick - lastSMTick >= SM_TICK)
		{
			lastSMTick = tick;
			StateMachine();
		}
		if(tick - lastCANTick >= CAN_TICK)
		{
			lastCANTick = tick;
			CAN_Handler_SendFrame();
		}
		if(ErrorFlag != 0) ErrorStateHandler();
	}
}
void StateMachine()
{
	switch(State)
	{
	case Init:
		LEDFlashInit();
		break;
	case Operational:
		CheckData();
		BalanceCell_1(false);
		BalanceCell_2(false);
		BalanceCell_3(false);
		break;
	case Balance:
		BalanceHandler();
		CheckData();
		break;
	case Error:
		ErrorStateHandler();
		RelayOFF();
		break;
	default:
		ErrorFlag = UNKWN_ERROR;
		ErrorStateHandler();
		RelayOFF();
		break;
	}
}
void CheckData()
{
	voltages[2] = roundf(ADC_Handler_GetVC3()*100)/100;
	voltages[1] = roundf(ADC_Handler_GetVC2()*100)/100;
	voltages[0] = roundf(ADC_Handler_GetVC1()*100)/100;

	temps[0] = ADC_Handler_GetTemp1();
	temps[1] = ADC_Handler_GetTemp2();
	temps[2] = ADC_Handler_GetTemp3();

	for(uint8_t i=0;i<3;i++)
	{
		//check voltages
		if(voltages[i]<CELL_MINIMUM_VOLT)
		{
			cellErrorCounter[i] += 50;
			if(cellErrorCounter[i]>=MAX_ERROR_COUNTER)
			{
				ErrorFlag = CELL_UNDERVOLT;
				ErrorStateHandler();
			}
		}
		else
		{
			if(cellErrorCounter[i]>0)
			{
				cellErrorCounter[i] -= 10;
			}
		}
		//check temps
		if(temps[i]>CELL_MAX_TEMP || temps[i]<=CELL_MIN_TEMP)
		{
			tempErrorCounter[i] += 50;
			if(tempErrorCounter[i]>MAX_ERROR_COUNTER)
			{
				ErrorFlag = CELL_OVERTEMP;
				ErrorStateHandler();
			}
		}
		else
		{
			if(tempErrorCounter[i]>0)
			{
				tempErrorCounter[i] -=10;
			}
		}
	}
	if(State == Operational)
	{
		//find highest voltage
		for(uint8_t i = 0; i<3;i++)
		{
			if(voltages[highestVoltage(voltages)] - voltages[i] >= MAX_VOLT_DIFF)
			{
				cellToBalance = highestVoltage(voltages)+1;
				State = Balance;
			}

		}
//		if(cellToBalance != 0)
//		{
//			float mean = 0;
//			for(uint8_t i=0; i<3;i++)
//			{
//				if(i != cellToBalance - 1)
//				{
//					mean+=voltages[i];
//				}
//			}
//			targetVoltage = mean/2;
//		}
	}
	if(ErrorFlag == 0)
	{
		RelayON();
	}
}

void ErrorStateHandler()
{
	State = Error;
	RelayOFF();
	BalanceCell_1(false);
	BalanceCell_2(false);
	BalanceCell_3(false);
	switch(ErrorFlag)
	{
	case CELL_UNDERVOLT:
		HAL_GPIO_WritePin(LED_VOLT_GPIO_Port, LED_VOLT_Pin, GPIO_PIN_SET);
		break;
	case CELL_OVERVOLT:

		break;
	case CELL_OVERTEMP:
		HAL_GPIO_WritePin(LED_TEMP_GPIO_Port, LED_TEMP_Pin, GPIO_PIN_SET);
		break;
	case PACK_OVERCURRENT:
		HAL_GPIO_WritePin(LED_CURR_GPIO_Port, LED_CURR_Pin, GPIO_PIN_SET);
		break;
	case UNKWN_ERROR:
		Error_Handler();
		break;
	default:
		Error_Handler();
		break;
	}
}
void LEDFlashInit()
{

	HAL_GPIO_WritePin(LED_BALANCE_GPIO_Port, LED_BALANCE_Pin, GPIO_PIN_SET);
	HAL_Delay((uint32_t) BOOT_DELAY/4);
	HAL_GPIO_WritePin(LED_VOLT_GPIO_Port, LED_VOLT_Pin, GPIO_PIN_SET);
	HAL_Delay((uint32_t) BOOT_DELAY/4);
	HAL_GPIO_WritePin(LED_TEMP_GPIO_Port, LED_TEMP_Pin, GPIO_PIN_SET);
	HAL_Delay((uint32_t) BOOT_DELAY/4);
	HAL_GPIO_WritePin(LED_CURR_GPIO_Port, LED_CURR_Pin, GPIO_PIN_SET);
	HAL_Delay((uint32_t) BOOT_DELAY/4);
	HAL_GPIO_WritePin(LED_BALANCE_GPIO_Port, LED_BALANCE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_VOLT_GPIO_Port, LED_VOLT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_TEMP_GPIO_Port, LED_TEMP_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_CURR_GPIO_Port, LED_CURR_Pin, GPIO_PIN_RESET);
	State = Operational;
	CheckData();
}
void BalanceHandler()
{
	State = Balance;
	CheckData();
//	if(targetVoltage == 0)
//	{
//		State = Operational;
//		cellToBalance = 0;
//	}
	if(tick - lastBlinkTick >= BALANCE_BLINK)
	{
		lastBlinkTick = HAL_GetTick();
		HAL_GPIO_TogglePin(LED_BALANCE_GPIO_Port, LED_BALANCE_Pin);
	}
//	if(tick - lastBalanceTick >= CYCLE_TICK)
//	{
//		lastBalanceTick = HAL_GetTick();
//		cycle++;
//		if(cycle == 10 && targetVoltage>0 && voltages[cellToBalance-1] <= targetVoltage)
//		{
//			State = Operational;
//			cellToBalance = 0;
//		}
//		if(cycle == 9)
//		{
//			BalanceCell_1(false);
//			BalanceCell_2(false);
//			BalanceCell_3(false);
//		}
//		if(cycle == 10)
//		{
//			cycle = 0;
//		}
//	}
//	switch(cellToBalance)
//	{
//	case 1:
//		if(cycle < 9)
//		{
//			BalanceCell_1(true);
//			BalanceCell_2(false);
//			BalanceCell_3(false);
//		}
//		break;
//	case 2:
//		if(cycle < 9)
//		{
//			BalanceCell_1(false);
//			BalanceCell_2(true);
//			BalanceCell_3(false);
//		}
//		break;
//	case 3:
//		if(cycle < 9)
//		{
//			BalanceCell_1(false);
//			BalanceCell_2(false);
//			BalanceCell_3(true);
//		}
//		break;
//	default:
//		cellToBalance = 0;
//		targetVoltage = 0;
//		HAL_GPIO_WritePin(LED_BALANCE_GPIO_Port, LED_BALANCE_Pin, GPIO_PIN_RESET);
//		BalanceCell_1(false);
//		BalanceCell_2(false);
//		BalanceCell_3(false);
//		State = Operational;
//		break;
//	}

}
uint8_t highestVoltage(float* voltages)
{
	uint8_t highestVoltage = 0;
	for(uint8_t i = 0; i<3;i++)
	{
		if(voltages[i] > voltages[highestVoltage])
		{
			highestVoltage = i;
		}
	}
	return highestVoltage;
}
void RelayON()
{
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);
}
void RelayOFF()
{
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);
}
void BalanceCell_1(bool ON)
{
	HAL_GPIO_WritePin(DSCH_1_GPIO_Port, DSCH_1_Pin, ON ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
void BalanceCell_2(bool ON)
{
	HAL_GPIO_WritePin(DSCH_2_GPIO_Port, DSCH_2_Pin, ON ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
void BalanceCell_3(bool ON)
{
	HAL_GPIO_WritePin(DSCH_3_GPIO_Port, DSCH_3_Pin, ON ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
