/************************************************
* @file    sysclk.h 
* @author  APBashara
* @date    6/2024
* 
* @brief   Clock Configuration Prototypes
***********************************************/

#include "stm32f415xx.h"

#define HSE_VALUE           16000000U // 16MHz External Crystal   

/**
 * @brief Configure the System Clock to 168MHz
 * 
 * @note HSE Clock is 8MHz
 */
void Sysclock_168();