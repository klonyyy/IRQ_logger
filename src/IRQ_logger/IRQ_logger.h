/*
 * IRQ_debug.h
 *
 *  Created on: Jan 23, 2022
 *      Author: Piotr Wasilewski (klonyyy)
 */

#ifndef SRC_IRQ_DEBUG_H_
#define SRC_IRQ_DEBUG_H_

#include "main.h"

/* EDIT THIS SECTION ACCORDINGLY */

#define MAX_ENTRIES 		500
#define IRQLOGGER_TIM 		TIM17

/* END */

/* critical sections defines */
#define DBG_ENTER_CRITICAL_SECTION  uint32_t primaskstate = __get_PRIMASK(); __disable_irq();
#define DBG_LEAVE_CRITICAL_SECTION  __set_PRIMASK(primaskstate);

typedef enum
{
	ENTER,
	LEAVE
}state_et;

typedef struct
{
	uint16_t timestamp_ms;
	uint16_t timestamp_us;
	uint8_t ID;
	uint8_t state;

}__attribute__((packed, aligned(1)))log_entry_st;

typedef union
{
	log_entry_st s;
	uint8_t bytes[sizeof(log_entry_st)];
}log_entry_ut;

log_entry_ut IRQ_logger_buff[MAX_ENTRIES];

/* init function - should be called on system startup, and contain your timestamp timer setup */
void IRQ_logger_init();
/* this function just increments millisecond counter - call it in the timestamp timer update interrupt (every 1ms) */
void IRQ_logger_millisecond();
/* adds a log to the log buffer */
void IRQ_logger_add_entry(uint8_t ID, state_et state);

/* these functions are used as interface in case you'd want to trigger or stop logging from inside of your code */
void IRQ_logger_set_log_on();
void IRQ_logger_set_log_off();
uint8_t IRQ_logger_get_log_state();
void IRQ_logger_set_idx(uint16_t idx);
uint16_t IRQ_logger_get_idx();


#endif /* SRC_IRQ_DEBUG_H_ */
