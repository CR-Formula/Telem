/************************************************
* @file    uart.c 
* @author  APBashara
* @date    6/2024
* 
* @brief   Basic UART Driver
***********************************************/

#include "stm32f415xx.h"

/**
 * @brief Initialize USART2
 * @note Baud rate = 115200
 * @note Uses Pins PA2 (Tx) and PA3 (Rx)
 */
void USART2_Init() {
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // Enable USART2 Clock
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enable GPIO A Clock

  USART2->CR1 &= ~USART_CR1_UE; // Disable USART

  GPIOA->MODER &= ~GPIO_MODER_MODE2 & ~GPIO_MODER_MODE3; // Clear PA2 and PA3
  GPIOA->MODER |= (0x2 << GPIO_MODER_MODE2_Pos) 
                | (0x2 << GPIO_MODER_MODE3_Pos); // Set PA2 and PA3 to TX and RX

  GPIOA->OSPEEDR |= (0x2 << GPIO_OSPEEDR_OSPEED2_Pos) 
                  | (0x2 << GPIO_OSPEEDR_OSPEED3_Pos); // Set PA2 and PA3 to High Speed

  GPIOA->AFR[0] |= (0x7 << GPIO_AFRL_AFSEL2_Pos) 
                  | (0x7 << GPIO_AFRL_AFSEL3_Pos); // Set PA2 and PA3 to AF7 (USART2)
  
  USART2->CR1 |= USART_CR1_TE | USART_CR1_RE; // Enable Transmitter and Receiver

  // baud rate = fCK / (8 * (2 - OVER8) * USARTDIV)
  // Page 989 of Reference Manual
  // 115200 @ 42MHz = 22.8125
  USART2->BRR |= (0xD << USART_BRR_DIV_Fraction_Pos);
  USART2->BRR |= (0x16 << USART_BRR_DIV_Mantissa_Pos);

  USART2->CR1 |= USART_CR1_UE; // Enable USART
}

/**
 * @brief Initialize USART3
 * @note Baud rate = 115200
 * @note Uses Pins PB10 (Tx) and PB11 (Rx)
 */
void USART3_Init() {
  RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // Enable USART3 Clock
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable GPIO B Clock

  USART3->CR1 &= ~USART_CR1_UE; // Disable USART

  GPIOB->MODER &= ~GPIO_MODER_MODE10 & ~GPIO_MODER_MODE11; // Clear PB10 and PB11
  GPIOB->MODER |= (0x2 << GPIO_MODER_MODE10_Pos) 
                | (0x2 << GPIO_MODER_MODE11_Pos); // Set PB10 and PB11 to TX and RX

  GPIOB->OSPEEDR |= (0x2 << GPIO_OSPEEDR_OSPEED10_Pos) 
                  | (0x2 << GPIO_OSPEEDR_OSPEED11_Pos); // Set PB10 and PB11 to High Speed

  GPIOB->AFR[1] |= (0x7 << GPIO_AFRH_AFSEL10_Pos) 
                  | (0x7 << GPIO_AFRH_AFSEL11_Pos); // Set PB10 and PB11 to AF7 (USART3)

  USART3->CR1 |= USART_CR1_TE | USART_CR1_RE; // Enable Transmitter and Receiver

  // baud rate = fCK / (8 * (2 - OVER8) * USARTDIV)
  // Page 989 of Reference Manual
  // 115200 @ 42MHz = 22.8125
  USART3->BRR |= (0xD << USART_BRR_DIV_Fraction_Pos);
  USART3->BRR |= (0x16 << USART_BRR_DIV_Mantissa_Pos);

  USART3->CR1 |= USART_CR1_UE; // Enable USART
}

/**
 * @brief Sends a byte over a USART
 * 
 * @param USART [USART_TypeDef*] USART to use to send message
 * @param byte [uint8_t] Byte to send
 */
void send_Byte(USART_TypeDef* USART, uint8_t byte) {
  USART->DR = byte;
  while (!(USART->SR & USART_SR_TC));
}

/**
 * @brief Sends a given string over USART
 * 
 * @param USART [USART_TypeDef*] USART to use to send message
 * @param string [uint8_t*] String to send
 */
void send_String(USART_TypeDef* USART, uint8_t *string) {
  int i = 0;
  while (string[i] != '\0') {
    send_Byte(USART, string[i]);
    i++;
  }
}