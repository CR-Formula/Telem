/************************************************
* @file    timer.h 
* @author  APBashara
* @date    6/2024
* 
* @brief   Timer Function Prototypes
***********************************************/

#include "stm32f415xx.h"

/**
 * @brief Initialize and start Timer 2
 * @note Timer 2 runs at APB1(42Mhz) * 2 = 84Mhz
 * @note Timer runs at fCK_PSC / (PSC + 1)
 */
void TIM2_Init();

/**
 * @brief Configure Timer for Run Time Stats
 * 
 */
void Timer_Stat_Init();

/**
 * @brief Get Timer Counter Value
 * 
 * @return uint32_t 
 */
uint32_t Get_Timer_Count();