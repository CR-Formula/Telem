/************************************************
* @file    gpio.c
* @author  APBashara
* @date    6/2024
* 
* @brief   Basic GPIO Driver
***********************************************/

#include "stm32f407xx.h"

/**
 * @brief Initialize the LED Pins
 * 
 * @note LEDs on the STM32F4-Disco board
 */
void LED_Init() {
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // Enable GPIO D Clock

  // Set LED Pins to Output mode
  // Pins reset to push pull mode
  GPIOD->MODER &= ~GPIO_MODER_MODE12 & ~GPIO_MODER_MODE13 
                & ~GPIO_MODER_MODE14 & ~GPIO_MODER_MODE15; // Clear LED Pins
  GPIOD->MODER |= (0x1 << GPIO_MODER_MODE12_Pos) | (0x1 << GPIO_MODER_MODE13_Pos) 
                | (0x1 << GPIO_MODER_MODE14_Pos) | (0x1 << GPIO_MODER_MODE15_Pos); // Set LED Pins to output
}

/**
 * @brief Toggle a given GPIO pin on or off
 * 
 * @param GPIO [GPIO_TypeDef*] GPIO Port to use
 * @param pin [uint8_t] Pin to toggle
 */
void Toggle_Pin(GPIO_TypeDef* GPIO, uint8_t pin) {
  GPIO->ODR ^= (1 << pin);
}

/**
 * @brief Set a given GPIO pin high (Set to 1)
 * 
 * @param GPIO [GPIO_TypeDef*] GPIO Port to use
 * @param pin [uint8_t] Pin to set high
 */
void Set_Pin(GPIO_TypeDef* GPIO, uint8_t pin) {
  GPIO->ODR |= (1 << pin);
}

/**
 * @brief Clear a given GPIO pin (Set to 0)
 * 
 * @param GPIO [GPIO_TypeDef*] GPIO Port to use
 * @param pin [uint8_t] Pin to set low
 */
void Clear_Pin(GPIO_TypeDef* GPIO, uint8_t pin) {
  GPIO->ODR &= ~(1 << pin);
}