/************************************************
* @file    timer.c
* @author  APBashara
* @date    6/2024
* 
* @brief   Basic Timer Driver
***********************************************/

#include "stm32f415xx.h"

/**
 * @brief Initialize and start Timer 2
 * @note Timer 2 runs at APB1(42Mhz) * 2 = 84Mhz
 * @note Timer runs at fCK_PSC / (PSC + 1)
 */
void TIM2_Init() {
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable TIM2 Clock

  TIM2->CR1 &= ~TIM_CR1_CEN; // Disable Timer
  TIM2->CR1 &= ~TIM_CR1_DIR; // Counting Up Direction

  NVIC_SetPriority(TIM2_IRQn, 2); // Set Priority to 2
  NVIC_EnableIRQ(TIM2_IRQn); // Enable TIM2 Interrupt

  // Configure and start the Timer
  TIM2->PSC = 8400 - 1; // Set Prescaler to 8399 (10KHz)
  TIM2->ARR = 1000; // Set Auto Reload Register to 1000

  TIM2->DIER |= TIM_DIER_UIE; // Enable Update Interrupt
  
  TIM2->CR1 |= TIM_CR1_CEN; // Enable Timer
}

void Timer_Stat_Init() {
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable TIM2 Clock
  
  TIM2->CR1 &= ~TIM_CR1_CEN; // Disable Timer
  // Count up and no clock division
  TIM2->CR1 &= ~TIM_CR1_DIR & ~TIM_CR1_CKD;

  TIM2->PSC = ((SystemCoreClock / 1000000) - 1); // Set Prescaler to 83 (1MHz)
  TIM2->ARR = 0xFFFFFFFF; // Set Auto Reload Register to max

  TIM2->CR1 |= TIM_CR1_CEN; // Enable Timer
}

uint32_t Get_Timer_Count() {
  return TIM2->CNT;
}