/************************************************
* @file    spi.h 
* @author  APBashara
* @date    6/2024
* 
* @brief   Prototype Functions for SPI Driver
***********************************************/

#include "stm32f415xx.h"

/**
 * @brief Initialize SPI2
 * @note CPOL = 1, CPHA = 0, MSB First, 8-bit Data Frame
 */
void SPI2_Init();

/**
 * @brief Send a byte over SPI
 * @note Does not handle CS pin
 * 
 * @param SPI [SPI_TypeDef*] SPI Peripheral to use
 * @param data [uint8_t] Data to write
 * @param CS [uint8_t] Chip Select Pin
 * @return [uint8_t] Data read from slave shift register
 */
uint8_t SPI_Write(SPI_TypeDef* SPI, uint8_t data);

/**
 * @brief Write a register on a SPI device
 * @note Designed for RFM95W LoRa Module
 * 
 * @param SPI [SPI_TypeDef*] SPI Peripheral to use
 * @param reg [uint8_t] Register Address
 * @param data [uint8_t] Data to write
 * @param CS [uint8_t] Chip Select Pin
 */
void SPI_Transmit_Frame(SPI_TypeDef* SPI, uint8_t *buf, uint16_t size, uint8_t CS);

/**
 * @brief Read a register on a SPI device
 * @note Designed for RFM95W LoRa Module
 * TODO: Need to set up read command
 * 
 * @param SPI [SPI_TypeDef*] SPI Peripheral to use
 * @param reg [uint8_t] Register Address
 * @param CS [uint8_t] Chip Select Pin
 * @return uint8_t Register Data
 */
uint8_t Read_Register(SPI_TypeDef* SPI, uint8_t reg, uint8_t CS);