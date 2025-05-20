/************************************************
* @file    gpio.c
* @author  APBashara
* @date    6/2024
* 
* @brief   Basic GPIO Driver
***********************************************/

#include "gpio.h"

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

void GPIO_Init() {
  // Enable GPIO A Clock
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

  // Set GPIO Pins to Output mode
  // Pins reset to push pull mode
  GPIOA->MODER &= ~GPIO_MODER_MODE8 & ~GPIO_MODER_MODE9; // Clear RST Pin
  GPIOA->MODER |= (0x1 << GPIO_MODER_MODE8_Pos) | (0x0 << GPIO_MODER_MODE9_Pos);
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED8 & ~GPIO_OSPEEDR_OSPEED9;
  GPIOA->OSPEEDR |= (0x1 << GPIO_OSPEEDR_OSPEED8_Pos) | (0x1 << GPIO_OSPEEDR_OSPEED9_Pos);
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD8 & ~GPIO_PUPDR_PUPD9;

  // TODO: GPIOB Config for GPS reset pin
}

GPIO_Status GPIO_EXTI_Init(GPIO_TypeDef* GPIO, uint8_t pin) {
  // Enable SYSCFG Clock
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  uint8_t port = 0;
  if (GPIO == GPIOA) {
    port = 0;
  } else if (GPIO == GPIOB) {
    port = 1;
  } else if (GPIO == GPIOC) {
    port = 2;
  } else if (GPIO == GPIOD) {
    port = 3;
  } else if (GPIO == GPIOE) {
    port = 4;
  } else if (GPIO == GPIOF) {
    port = 5;
  } else if (GPIO == GPIOG) {
    port = 6;
  } else if (GPIO == GPIOH) {
    port = 7;
  } else if (GPIO == GPIOI) {
    port = 8;
  }
  else {
    return GPIO_ERROR;
  }

  // Configure EXTI Line
  SYSCFG->EXTICR[pin / 4] &= ~(0xF << ((pin % 4) * 4));
  SYSCFG->EXTICR[pin / 4] |= (port << ((pin % 4) * 4));

  // Configure EXTI Trigger
  EXTI->IMR |= (0x1 << pin); // Enable Interrupt Mask
  EXTI->RTSR |= (0x1 << pin); // Enable Rising Edge Trigger
  EXTI->FTSR &= ~(0x1 << pin); // Disable Falling Edge Trigger

  return GPIO_OK;
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