/************************************************
* @file    gpio.h 
* @author  APBashara
* @date    6/2024
* 
* @brief   GPIO Function Prototypes
***********************************************/

#include "stm32f415xx.h"

/* Structs and Enums --------------------------------------------------------*/
typedef enum {
    GPIO_OK,
    GPIO_ERROR,
} GPIO_Status;

/* Function Prototypes ------------------------------------------------------*/

/**
 * @brief Initialize the LED Pins
 * 
 * @note LEDs on the STM32F4-Disco board
 */
void LED_Init();

/**
 * @brief Initialize GPIO Pins used in the Telem system
 * @note PA8 LoRa RST, PA9 LoRa INT
 * 
 */
void GPIO_Init();

/**
 * @brief Configure an EXTI line for a given GPIO pin
 * @note Must call NVIC Functions after this function
 * @note Rising Edge Interrupt
 * 
 * @param GPIO [GPIO_TypeDef*] GPIO Port to use
 * @param pin [uint8_t] Pin to configure
 */
GPIO_Status GPIO_EXTI_Init(GPIO_TypeDef* GPIO, uint8_t pin);

/**
 * @brief Toggle a given GPIO pin on or off
 * 
 * @param GPIO [GPIO_TypeDef*] GPIO Port to use
 * @param pin [uint8_t] Pin to toggle
 */
void Toggle_Pin(GPIO_TypeDef* GPIO, uint8_t pin);

/**
 * @brief Set a given GPIO pin high (Set to 1)
 * 
 * @param GPIO [GPIO_TypeDef*] GPIO Port to use
 * @param pin [uint8_t] Pin to set high
 */
void Set_Pin(GPIO_TypeDef* GPIO, uint8_t pin);

/**
 * @brief Clear a given GPIO pin (Set to 0)
 * 
 * @param GPIO [GPIO_TypeDef*] GPIO Port to use
 * @param pin [uint8_t] Pin to set low
 */
void Clear_Pin(GPIO_TypeDef* GPIO, uint8_t pin);