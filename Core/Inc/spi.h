/************************************************
* @file    spi.h 
* @author  APBashara
* @date    6/2024
* 
* @brief   Prototype Functions for SPI Driver
***********************************************/

#include "stm32f415xx.h"
#include <stdint.h>
#include <stddef.h>

/* Macros -------------------------------------------------------------------*/
/* Structs and Enums --------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/

/**
 * @brief Initialize SPI2
 * @note CPOL = 0, CPHA = 0, MSB First, 8-bit Data Frame
 */
void SPI2_Init();

/**
 * @brief Transmit a byte over SPI
 * @note Does not handle CS pin
 * 
 * @param SPI [SPI_TypeDef*] SPI Peripheral to use
 * @param data [uint8_t] Data to write
 * @return [uint8_t] Data read from slave shift register
 */
uint8_t SPI_Transmit(SPI_TypeDef* SPI, uint8_t data);