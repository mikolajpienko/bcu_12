/*
 * CAN_Handler.h
 *
 *  Created on: Sep 28, 2021
 *      Author: Micza
 */

#ifndef INC_CAN_HANDLER_H_
#define INC_CAN_HANDLER_H_

#define LVBMS_CELLS_ID			0x703
#define LVBMS_CELLS_PERIOD		200
#define LVBMS_CELLS_DLC		8

#define LVBMS_VC1_BIT_OFS		0
#define LVBMS_VC1_VALUE_TYPE		_uint13_t
#define LVBMS_VC1_MUL		1
#define LVBMS_VC1_DIV		1000

#define LVBMS_VC2_BIT_OFS		13
#define LVBMS_VC2_VALUE_TYPE		_uint13_t
#define LVBMS_VC2_MUL		1
#define LVBMS_VC2_DIV		1000

#define LVBMS_VC3_BIT_OFS		26
#define LVBMS_VC3_VALUE_TYPE		_uint13_t
#define LVBMS_VC3_MUL		1
#define LVBMS_VC3_DIV		1000

#define LVBMS_T1_BIT_OFS		40
#define LVBMS_T1_VALUE_TYPE		_int8_t
#define LVBMS_T1_MUL		1
#define LVBMS_T1_DIV		1

#define LVBMS_T2_BIT_OFS		48
#define LVBMS_T2_VALUE_TYPE		_int8_t
#define LVBMS_T2_MUL		1
#define LVBMS_T2_DIV		1

#define LVBMS_T3_BIT_OFS		56
#define LVBMS_T3_VALUE_TYPE		_int8_t
#define LVBMS_T3_MUL		1
#define LVBMS_T3_DIV		1

void CAN_Handler_Init(void);
void CAN_Handler_SendFrame(void);


#endif /* INC_CAN_HANDLER_H_ */
