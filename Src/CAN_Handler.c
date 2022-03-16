/*
 * CAN_Handler.c
 *
 *  Created on: Sep 28, 2021
 *      Author: Micza
 */
#include "CAN_LIB.h"
#include "CAN_Handler.h"
#include "ADC_Handler.h"
struct can_frame bmsFrame;
struct can_value vc1, vc2, vc3, temp1, temp2, temp3;
void CAN_Handler_Init()
{
	CAN_Init();

	CAN_InitFrame(&bmsFrame, LVBMS_CELLS_ID, LVBMS_CELLS_PERIOD, LVBMS_CELLS_DLC);

	CAN_InitValue(&bmsFrame, &temp1, LVBMS_T1_BIT_OFS, LVBMS_T1_MUL, LVBMS_T1_DIV, 0, LVBMS_T1_VALUE_TYPE);
	CAN_InitValue(&bmsFrame, &temp2, LVBMS_T2_BIT_OFS, LVBMS_T2_MUL, LVBMS_T2_DIV, 0, LVBMS_T2_VALUE_TYPE);
	CAN_InitValue(&bmsFrame, &temp3, LVBMS_T3_BIT_OFS, LVBMS_T3_MUL, LVBMS_T3_DIV, 0, LVBMS_T3_VALUE_TYPE);

	CAN_InitValue(&bmsFrame, &vc1, LVBMS_VC1_BIT_OFS, LVBMS_VC1_MUL, LVBMS_VC1_DIV, 0, LVBMS_VC1_VALUE_TYPE);
	CAN_InitValue(&bmsFrame, &vc2, LVBMS_VC2_BIT_OFS, LVBMS_VC2_MUL, LVBMS_VC2_DIV, 0, LVBMS_VC2_VALUE_TYPE);
	CAN_InitValue(&bmsFrame, &vc3, LVBMS_VC3_BIT_OFS, LVBMS_VC3_MUL, LVBMS_VC3_DIV, 0, LVBMS_VC3_VALUE_TYPE);

}
void CAN_Handler_SendFrame()
{
	CAN_WriteValueFloat(&temp1, ADC_Handler_GetTemp1());
	CAN_WriteValueFloat(&temp2, ADC_Handler_GetTemp2());
	CAN_WriteValueFloat(&temp3, ADC_Handler_GetTemp3());

	CAN_WriteValueFloat(&vc1, ADC_Handler_GetVC1());
	CAN_WriteValueFloat(&vc2, ADC_Handler_GetVC2());
	CAN_WriteValueFloat(&vc3, ADC_Handler_GetVC3());

	CAN_SendFrame(&bmsFrame);
}
