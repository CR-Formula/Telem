#include "stm32f415xx.h"

// TODO: Implement USART3 LIN Bus

/**
 * @brief Initialize USART6
 * @note Baud rate = 115200
 */
void static inline USART6_Init() {
  RCC->APB2ENR |= RCC_APB2ENR_USART6EN; // Enable USART6 Clock
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Enable GPIO C Clock

  USART6->CR1 &= ~USART_CR1_UE; // Disable USART

  GPIOC->MODER &= ~GPIO_MODER_MODE6 & ~GPIO_MODER_MODE7; // Clear PC6 (Tx) and PC7 (Rx)
  GPIOC->MODER |= (0x2 << GPIO_MODER_MODE6_Pos) 
                | (0x2 << GPIO_MODER_MODE7_Pos); // Set PC6 and PC7 to Alternate Function

  GPIOC->OSPEEDR |= (0x2 << GPIO_OSPEEDR_OSPEED6_Pos) 
                  | (0x2 << GPIO_OSPEEDR_OSPEED7_Pos); // Set PC6 and PC7 to High Speed

  GPIOC->AFR[0] |= (0x8 << GPIO_AFRL_AFSEL6_Pos) 
                  | (0x8 << GPIO_AFRL_AFSEL7_Pos); // Set PC6 (Tx) and PC7 (Rx) to AF8 (USART6)
  
  USART6->CR1 |= USART_CR1_TE | USART_CR1_RE; // Enable Transmitter and Receiver

  // baud rate = fCK / (8 * (2 - OVER8) * USARTDIV)
  // Page 989 of Reference Manual
  // 115200 @ 82MHz = 45.5625 = 45 and (.5625 * 16) = 9
  USART6->BRR |= (0x9 << USART_BRR_DIV_Fraction_Pos);
  USART6->BRR |= (0x2D << USART_BRR_DIV_Mantissa_Pos);

  USART6->CR1 |= USART_CR1_UE; // Enable USART
}

/**
 * @brief Sends a byte over a USART
 * 
 * @param USART USART to send byte over
 * @param byte Byte to send
 */
void static inline send_Byte(USART_TypeDef* USART, uint8_t byte) {
  USART->DR = byte;
  while (!(USART->SR & USART_SR_TC));
}

/**
 * @brief send a given string over a USART
 * 
 * @param USART USART to send byte over
 * @param string String to send
 */
void static inline send_String(USART_TypeDef* USART, uint8_t *string) {
  for (uint16_t i = 0; i < sizeof(string); i++) {
    send_Byte(USART, string[i]);
  }
}
