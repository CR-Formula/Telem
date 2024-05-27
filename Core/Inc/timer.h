#include "stm32f415xx.h"

/**
 * @brief Initialize and start Timer 2
 * @note Timer 2 runs at APB1(42Mhz) * 2 = 84Mhz
 * @note Timer runs at fCK_PSC / (PSC + 1)
 */
void static inline TIM2_Init() {
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