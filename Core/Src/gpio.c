/************************************************
* @file    gpio.c
* @author  APBashara
* @date    6/2024
* 
* @brief   Basic GPIO Driver
***********************************************/

#include "stm32f415xx.h"

/**
 * @brief Initialize the LED Pins
 * 
 * @note LEDs on the STM32F4-Disco board
 */
void LED_Init() {
  // Enable GPIO C Clock
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

  // Set LED Pins to Output mode
  // Pins reset to push pull mode
  GPIOC->MODER &= ~GPIO_MODER_MODE13; // Clear LED Pin
  GPIOC->MODER |= (0x1 << GPIO_MODER_MODE13_Pos); // Set LED Pin to output
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
  GPIO->BSRR |= (1 << pin);
}

/**
 * @brief Clear a given GPIO pin (Set to 0)
 * 
 * @param GPIO [GPIO_TypeDef*] GPIO Port to use
 * @param pin [uint8_t] Pin to set low
 */
void Clear_Pin(GPIO_TypeDef* GPIO, uint8_t pin) {
  GPIO->BSRR |= (1 << (pin+16));
}