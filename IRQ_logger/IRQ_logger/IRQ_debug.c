/*
 * IRQ_debug.c
 *
 *  Created on: Jan 23, 2022
 *      Author: Piotr Wasilewski (klonyyy)
 */
#include "IRQ_debug.h"

#pragma GCC push_options
#pragma GCC optimize ("-O3")

volatile uint32_t currentIdx;
volatile uint32_t milisecond;
volatile uint8_t enabled = 0;

static inline uint16_t _us(void)
{
	return DEBUG_TIM->CNT;
}

static inline uint32_t _ms(void)
{
	return milisecond;
}

void dbg_uartInit()
{
	LL_DMA_DisableChannel(DEBUG_UART_DMA, DEBUG_UART_DMA_TX);
	LL_DMA_ConfigAddresses(DEBUG_UART_DMA, DEBUG_UART_DMA_TX, (uint32_t)&debug_buff->bytes[0], (uint32_t)&DEBUG_UART->TDR, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_EnableIT_TC(DEBUG_UART_DMA, DEBUG_UART_DMA_TX);
	LL_USART_Disable(DEBUG_UART);
	LL_USART_EnableDMAReq_TX(DEBUG_UART);
	LL_USART_EnableIT_RXNE_RXFNE(DEBUG_UART);
	LL_USART_Enable(DEBUG_UART);
}
void dbg_uartSendBuffer()
{
	LL_DMA_SetDataLength(DEBUG_UART_DMA, DEBUG_UART_DMA_TX, dbg_getIdx()*sizeof(debug_entry));
	LL_DMA_EnableChannel(DEBUG_UART_DMA, DEBUG_UART_DMA_TX);
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
void dbg_control()
{
	if(LL_USART_IsActiveFlag_RXNE_RXFNE(DEBUG_UART))
	{
		if(LL_USART_ReceiveData8(DEBUG_UART) == '@')dbg_setDebugOn();
		else if(LL_USART_ReceiveData8(DEBUG_UART) == '!')
		{
			dbg_setDebugOff();
			dbg_uartSendBuffer();
			dbg_setIdx(0);
		}
	}
}
void dbg_addIRQ(uint8_t ID, state_e enter)
{
	DBG_ENTER_CRITICAL_SECTION;
	if(currentIdx < MAX_ENTRIES && enabled == 1)
	{
		debug_buff[currentIdx].s.timestamp_ms = _ms();
		debug_buff[currentIdx].s.timestamp_us = _us();
		debug_buff[currentIdx].s.ID = ID;
		debug_buff[currentIdx].s.enter = enter;
		currentIdx++;
	}
	DBG_LEAVE_CRITICAL_SECTION;
}

#pragma GCC pop_options
