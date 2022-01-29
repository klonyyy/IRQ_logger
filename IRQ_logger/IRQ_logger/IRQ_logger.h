/*
 * IRQ_debug.h
 *
 *  Created on: Jan 23, 2022
 *      Author: Piotr Wasilewski (klonyyy)
 */

#ifndef SRC_IRQ_DEBUG_H_
#define SRC_IRQ_DEBUG_H_

#include "main.h"

#define IRQ_DEBUG_OFF 0

/* EDIT THIS SECTION ACCORDINGLY */
#define MAX_ENTRIES 	500
#define DEBUG_TIM 		TIM17
/* END */

/* critical setions defines */
#define DBG_ENTER_CRITICAL_SECTION  uint32_t primaskstate = __get_PRIMASK(); __disable_irq();
#define DBG_LEAVE_CRITICAL_SECTION  __set_PRIMASK(primaskstate);

typedef enum
{
	ENTER,
	LEAVE
}state_e;

typedef struct
{
	uint16_t timestamp_ms;
	uint16_t timestamp_us;
	uint8_t ID;
	uint8_t enter;

}__attribute__((packed, aligned(1)))debug_entry;

typedef union
{
	debug_entry s;
	uint8_t bytes[sizeof(debug_entry)];
}debug_entry_t;

debug_entry_t debug_buff[MAX_ENTRIES];

void dbg_milisecond();
void dbg_init();
void dbg_uartSendBuffer();
void dbg_setDebugOn();
void dbg_setDebugOff();
uint8_t dbg_getDebugState();
void dbg_setIdx(uint16_t idx);
uint16_t dbg_getIdx();
void dbg_control();
void dbg_addIRQ(uint8_t ID, state_e enter);

#endif /* SRC_IRQ_DEBUG_H_ */
