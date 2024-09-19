/************************************************
* @file    lin.c 
* @author  APBashara
* @date    9/2024
* 
* @brief   LIN driver implementation
***********************************************/

#include "lin.h"
#include "stm32f415xx.h"

LIN_Status LIN_Init() {
    // Set Clocks
    RCC->APB2ENR |= RCC_APB2ENR_USART6EN; // Enable USART6 Clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Enable GPIO C Clock

    USART6->CR1 &= ~USART_CR1_UE; // Disable USART

    // Configure GPIO
    GPIOC->MODER &= ~GPIO_MODER_MODE6 & ~GPIO_MODER_MODE7; // Clear PC6 and PC7
    GPIOC->MODER |= (0x2 << GPIO_MODER_MODE6_Pos) 
                 | (0x2 << GPIO_MODER_MODE7_Pos);
    GPIOC->AFR[0] |= (0x8 << GPIO_AFRL_AFSEL6_Pos) 
                  | (0x8 << GPIO_AFRL_AFSEL7_Pos); // Set PC6 and PC7 to AF8 (USART6)
    GPIOC->OSPEEDR |= (0x2 << GPIO_OSPEEDR_OSPEED6_Pos) 
                   | (0x2 << GPIO_OSPEEDR_OSPEED7_Pos); // Set PC6 and PC7 to High Speed

    // Configure USART6 for LIN
    USART6->CR2 |= USART_CR2_LINEN; // Enable LIN Mode
    USART6->CR2 &= ~USART_CR2_CLKEN & (0x0 << USART_CR2_STOP);
    USART6->CR3 &= ~USART_CR3_SCEN & ~USART_CR3_HDSEL & ~USART_CR3_IREN;
    USART6->CR1 |= USART_CR1_TE | USART_CR1_RE; // Enable Transmitter and Receiver

    // baud rate = fCK / (16 * USARTDIV)
    // Page 981 of Reference Manual
    // 115200 @ 84MHz = 45.5729166667
    USART2->BRR |= (0xD << USART_BRR_DIV_Fraction_Pos);
    USART2->BRR |= (0x16 << USART_BRR_DIV_Mantissa_Pos);

    USART6->CR1 |= USART_CR1_UE; // Enable USART
    return LIN_OK;
}