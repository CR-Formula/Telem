/************************************************
* @file    uart.h 
* @author  APBashara
* @date    6/2024
* 
* @brief   UART Function Prototypes
***********************************************/

#include "stm32f407xx.h"

/**
 * @brief Initialize USART2
 * @note Baud rate = 115200
 * @note Uses Pins PA2 (Tx) and PA3 (Rx)
 */
void USART2_Init();
/**
 * @brief Initialize USART3
 * @note Baud rate = 115200
 * @note Uses Pins PB10 (Tx) and PB11 (Rx)
 */
void USART3_Init();

/**
 * @brief Sends a byte over a USART
 * 
 * @param USART [USART_TypeDef*] USART to use to send message
 * @param byte [uint8_t] Byte to send
 */
void send_Byte(USART_TypeDef* USART, uint8_t byte);

/**
 * @brief Sends a given string over USART
 * 
 * @param USART [USART_TypeDef*] USART to use to send message
 * @param string [uint8_t*] String to send
 */
void send_String(USART_TypeDef* USART, uint8_t *string);