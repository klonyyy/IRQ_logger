/*
 * IRQ_debug.c
 *
 *  Created on: Jan 23, 2022
 *      Author: Piotr Wasilewski (klonyyy)
 */
#include <IRQ_logger.h>

#pragma GCC push_options
#pragma GCC optimize ("-O3")

volatile uint32_t currentIdx;
volatile uint32_t milisecond;
volatile uint8_t enabled = 0;

void dbg_init()
{
	LL_TIM_EnableIT_UPDATE(DEBUG_TIM);
	LL_TIM_EnableCounter(DEBUG_TIM);
}
void dbg_milisecond()
{
	milisecond++;
}
void dbg_setDebugOn()
{
	enabled = 1;
}
void dbg_setDebugOff()
{
	enabled = 0;
}
uint8_t dbg_getDebugState()
{
	return enabled;
}
void dbg_setIdx(uint16_t idx)
{
	currentIdx = idx;
}
uint16_t dbg_getIdx()
{
	return currentIdx;
}

void dbg_addIRQ(uint8_t ID, state_e enter)
{
	DBG_ENTER_CRITICAL_SECTION;
	if(currentIdx < MAX_ENTRIES && enabled == 1)
	{
		debug_buff[currentIdx].s.timestamp_ms = milisecond;
		debug_buff[currentIdx].s.timestamp_us = DEBUG_TIM->CNT;
		debug_buff[currentIdx].s.ID = ID;
		debug_buff[currentIdx].s.enter = enter;
		currentIdx++;
	}
	DBG_LEAVE_CRITICAL_SECTION;
}

#pragma GCC pop_options
