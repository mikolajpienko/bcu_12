/*
 * App.h
 *
 *  Created on: Sep 26, 2021
 *      Author: Micza
 */

#ifndef INC_APP_H_
#define INC_APP_H_

#include "stdio.h"
#include "stdbool.h"

#define MAX_ERROR_COUNTER 500
#define CELL_MINIMUM_VOLT 2.9
#define CELL_MAX_TEMP 60
#define CELL_MIN_TEMP -20
#define SM_TICK 10
#define CAN_TICK 100
#define BALANCE_BLINK 500
#define MAX_VOLT_DIFF 0.2
#define CYCLE_TICK 2000

enum State
{
	Init,
	Operational,
	Balance,
	Error
}State;

enum ErrorFlag
{
	NO_ERROR,
	CELL_UNDERVOLT,
	CELL_OVERVOLT,
	CELL_OVERTEMP,
	PACK_OVERCURRENT,
	UNKWN_ERROR
}ErrorFlag;

uint32_t tick = 0;
uint32_t lastSMTick = 0;
uint32_t lastCANTick = 0;
uint32_t lastBlinkTick = 0;
uint32_t lastBalanceTick = 0;

uint16_t cellErrorCounter[3];
uint16_t tempErrorCounter[3];
uint16_t currErrorCounter = 0;

uint8_t cellToBalance = 0;
uint8_t cycle = 0;

float temps[3];

float voltages[3];

float targetVoltage = 0;
int8_t CalcTemp(uint32_t ADC_val);

void ApplicationInit(void);
void StateMachine(void);
void LEDFlashInit(void);
void CheckData(void);
void ErrorStateHandler(void);
void BalanceHandler(void);
void RelayON(void);
void RelayOFF(void);
void BalanceCell_1(bool ON);
void BalanceCell_2(bool ON);
void BalanceCell_3(bool ON);
uint8_t highestVoltage(float* voltages);
#endif /* INC_APP_H_ */
