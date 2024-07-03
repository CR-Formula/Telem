/************************************************
* @file    sysclk.c
* @author  APBashara
* @date    6/2024
* 
* @brief   Basic Clock Configurations
***********************************************/

#include "stm32f407xx.h"

/**
 * @brief Configure the System Clock to 168MHz
 * 
 * @note HSE Clock is 8MHz
 */
void Sysclock_168() {
    RCC->CR |= RCC_CR_HSEON; // Enable HSE Clock
    while (!(RCC->CR & RCC_CR_HSERDY)); // Wait until HSE is ready

    RCC->APB1ENR |= RCC_APB1ENR_PWREN; // Enable Power Interface Clock
    PWR->CR |= PWR_CR_VOS; // Set Scale 1 mode (max clock frequency)

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1; // Set AHB Prescaler to 1
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4; // Set APB1 Prescaler to 4
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; // Set APB2 Prescaler to 2

    RCC->PLLCFGR = 7 << RCC_PLLCFGR_PLLQ_Pos; // Set PLLQ to 7
    RCC->PLLCFGR |= 0 << RCC_PLLCFGR_PLLP_Pos; // Set PLLP to 2
    RCC->PLLCFGR |= 336 << RCC_PLLCFGR_PLLN_Pos; // Set PLLN to 336
    RCC->PLLCFGR |= 8 << RCC_PLLCFGR_PLLM_Pos; // Set PLLM to 8
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE; // Set PLL Source to HSE

    FLASH->ACR |= FLASH_ACR_PRFTEN; // Enable Prefetch Buffer
    FLASH->ACR |= FLASH_ACR_ICEN; // Enable Instruction Cache
    FLASH->ACR |= FLASH_ACR_DCEN; // Enable Data Cache
    FLASH->ACR |= FLASH_ACR_LATENCY_5WS; // Set Flash Latency to 5 Wait States

    RCC->CR |= RCC_CR_PLLON; // Enable PLL
    while (!(RCC->CR & RCC_CR_PLLRDY)); // Wait until PLL is ready

    RCC->CFGR |= RCC_CFGR_SW_PLL; // Set PLL as System Clock
    while (!(RCC->CFGR & RCC_CFGR_SWS_PLL)); // Wait until PLL is System Clock
    
    SystemCoreClock = 168000000; // Set System Clock to 168MHz
}