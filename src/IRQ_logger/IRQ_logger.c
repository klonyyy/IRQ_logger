/*
 * IRQ_debug.c
 *
 *  Created on: Jan 23, 2022
 *      Author: Piotr Wasilewski (klonyyy)
 */
#include <IRQ_logger.h>

#pragma GCC push_options
#pragma GCC optimize ("-O3")

volatile uint32_t current_idx;
volatile uint32_t millisecond;
volatile uint8_t logging_enabled = 0;

void IRQ_logger_init()
{
	/* if not using CUBE MX just place your timer init routine here */
	LL_TIM_EnableIT_UPDATE(IRQLOGGER_TIM);
	LL_TIM_EnableCounter(IRQLOGGER_TIM);
}
void IRQ_logger_milisecond()
{
	millisecond++;
}
void IRQ_logger_add_entry(uint8_t ID, state_et state)
{
	DBG_ENTER_CRITICAL_SECTION;
	if(current_idx < MAX_ENTRIES && logging_enabled == 1)
	{
		IRQ_logger_buff[current_idx].s.timestamp_ms = millisecond;
		IRQ_logger_buff[current_idx].s.timestamp_us = IRQLOGGER_TIM->CNT;
		IRQ_logger_buff[current_idx].s.ID = ID;
		IRQ_logger_buff[current_idx].s.state = state;
		current_idx++;
	}
	DBG_LEAVE_CRITICAL_SECTION;
}


void IRQ_logger_set_debug_on()
{
	logging_enabled = 1;
}
void IRQ_logger_set_debug_off()
{
	logging_enabled = 0;
}
uint8_t IRQ_logger_get_log_state()
{
	return logging_enabled;
}
void IRQ_logger_set_idx(uint16_t idx)
{
	current_idx = idx;
}
uint16_t IRQ_logger_get_idx()
{
	return current_idx;
}


#pragma GCC pop_options
