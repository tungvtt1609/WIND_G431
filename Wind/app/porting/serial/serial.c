/*
 * serial.c
 *
 *  Created on: Sep 12, 2023
 *      Author: TungVT
 */

#include <serial/serial.h>
#include "main.h"
#include "lib/EC200_uart.h"

#define USE_FIFO

// Porting hardware
// extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

// static bool OpenUart1(int baudrate);
// static bool SendByteUart1(uint8_t byte);
// static bool ReceiveByteUart1(uint8_t *byte);

static bool OpenUart2(int baudrate);
static bool SendByteUart2(uint8_t byte);
static bool ReceiveByteUart2(uint8_t *byte);

static bool OpenUart3(int baudrate);
static bool SendByteUart3(uint8_t byte);
static bool ReceiveByteUart3(uint8_t *byte);

typedef struct stm_uart_obj
{
	serial_obj serial_port;
	// for porting hardware
	uint16_t uart_port;
	bool (*OpenUart)(int baudrate);
	bool (*SendByte)(uint8_t byte);
	bool (*ReceiveByte)(uint8_t *byte);
	// for platform
	fifo *tx_buf;
	fifo *rx_buf;
} stm_uart_obj;

static uint8_t uart2_data_rx_isr_buff[2];
#ifndef USE_FIFO
static uint8_t uart3_data_rx_isr_buff[2];
#endif

static stm_uart_obj uart_list[DEV_SERIAL] = {
	{{0, EC200_UART_BAUDRATE, 0, 0, 0}, 1, &OpenUart2, &SendByteUart2, &ReceiveByteUart2, null, null},
	{{1, 9600, 0, 0, 0}, 1, &OpenUart3, &SendByteUart3, &ReceiveByteUart3, null, null}};

/*=============================================================UART 2=====================================================================*/

static bool OpenUart2(int baudrate)
{
	huart2.Init.BaudRate = baudrate;
	if (HAL_UART_Init(&huart2) != HAL_OK)
		return false;
	HAL_UART_Receive_IT(&huart2, uart2_data_rx_isr_buff, 1);

	return true;
}

static bool SendByteUart2(uint8_t byte)
{
	HAL_StatusTypeDef ret;
	ret = HAL_UART_Transmit(&huart2, &byte, 1, 10);
	if (ret == HAL_OK)
		return true;
	return false;
}

void SendStringUart2(uint8_t *dat)
{
	int i=0;
	while(dat[i] != null)
	{
		SendByteUart2(dat[i]);
		i++;
	}
}

// unuse funtion ReceiveByteUart1 to read, use HAL_UART_RxCpltCallback interrupt to receive and put to fifo
static bool ReceiveByteUart2(uint8_t *byte)
{
	HAL_StatusTypeDef ret;
	ret = HAL_UART_Receive(&huart2, byte, 1, 0);
	if (ret == HAL_OK)
		return true;
	return false;
}

/*=============================================================UART 3=====================================================================*/

static bool OpenUart3(int baudrate)
{
	huart3.Init.BaudRate = baudrate;
	if (HAL_UART_Init(&huart3) != HAL_OK)
		return false;
#ifdef USE_FIFO
	if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_8_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_EnableFifoMode(&huart3) != HAL_OK)
	{
		Error_Handler();
	}
#else
	HAL_UART_Receive_IT(&huart3, uart3_data_rx_isr_buff, 1);
#endif
	return true;
}

static bool SendByteUart3(uint8_t byte)
{
	HAL_StatusTypeDef ret;
	ret = HAL_UART_Transmit(&huart3, &byte, 1, 10);
	if (ret == HAL_OK)
		return true;
	return false;
}

// unuse funtion ReceiveByteUart1 to read, use HAL_UART_RxCpltCallback interrupt to receive and put to fifo
static bool ReceiveByteUart3(uint8_t *byte)
{
	HAL_StatusTypeDef ret;
	if ((huart3.Instance->ISR & USART_ISR_RXNE_RXFNE) != 0)
	{
		ret = HAL_UART_Receive(&huart3, byte, 1, 0);
		if (ret == HAL_OK)
			return true;
	}
	return false;
}

/*==================================================================================================================================*/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

#ifndef USE_FIFO
	if (huart == &huart3)
	{
		fifo *rx = uart_list[1].rx_buf;
		HAL_UART_Receive_IT(huart, uart3_data_rx_isr_buff, 1);
		rx->Put(rx, uart3_data_rx_isr_buff);
		uart_list[1].serial_port.length_received = rx->data_len;
		return;
	}
#endif

	if (huart == &huart2)
	{
		EC200_UART_Handler(uart2_data_rx_isr_buff[0]);
		HAL_UART_Receive_IT(huart, uart2_data_rx_isr_buff, 1);
	}

}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
#ifndef USE_FIFO
	if (huart == &huart3)
	{
		HAL_UART_DeInit(&huart3);
		HAL_UART_Init(&huart3);
		HAL_UART_Receive_IT(&huart3, uart3_data_rx_isr_buff, 1);
	}
#endif
	if (huart == &huart2)
	{
		HAL_UART_DeInit(&huart2);
		HAL_UART_Init(&huart2);
	}

}

void SerialHandle(serial_obj *serial)
{
	uint8_t data;
	bool ret;
	fifo *tx = uart_list[serial->port].tx_buf;
	fifo *rx = uart_list[serial->port].rx_buf;
send:
	if (tx->data_len > 0)
	{
		tx->Peak(tx, &data);
		ret = uart_list[serial->port].SendByte(data);
		if (ret == true)
		{
			tx->Get(tx, &data);
			goto send;
		}
	}
#ifdef USE_FIFO
read:
	if (uart_list[serial->port].ReceiveByte(&data) == true)
	{
		rx->Put(rx, &data);
		goto read;
	}
	serial->length_received = rx->data_len;
	serial->length_send = tx->data_len;
#endif
}
// End porting hardware

serial_obj *create_serial(uint16_t port, uint32_t baudrate, uint16_t tx_size, uint16_t rx_size)
{
	serial_obj *serial;
	serial = &(uart_list[port].serial_port);
	serial->port = port;
	uart_list[port].tx_buf = create_fifo(tx_size, 1);
	uart_list[port].rx_buf = create_fifo(rx_size, 1);
	if (uart_list[port].tx_buf == null || uart_list[port].rx_buf == null)
		while (1)
			;

	serial->tx_size = tx_size;
	serial->rx_size = rx_size;
	serial->baudrate = baudrate;
	uart_list[port].OpenUart(baudrate);
	return serial;
}

bool SerialSendBuf(serial_obj *serial, uint8_t *data, uint16_t len)
{
	bool ret;
	fifo *tx = uart_list[serial->port].tx_buf;
	ret = tx->Puts(tx, data, len);
	serial->length_send = tx->data_len;

	return ret;
}

uint16_t SerialReadBuf(serial_obj *serial, uint8_t *data, uint16_t len)
{
	fifo *rx = uart_list[serial->port].rx_buf;
	if (rx->Gets(rx, data, len) == true)
	{
		serial->length_received = rx->data_len;
		return len;
	}
	return 0;
}
