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
typedef enum {
    SPI_ERROR,
    SPI_OK,
} SPI_Status;

/* Function Prototypes ------------------------------------------------------*/

/**
 * @brief Initialize SPI2
 * @note CPOL = 0, CPHA = 0, MSB First, 8-bit Data Frame
 */
void SPI2_Init();

/**
 * @brief Transmit a byte over SPI
 * @note Must Toggle CS pin manually
 * @note Assumes 8-bit registers
 * 
 * @param SPI [SPI_TypeDef*] SPI Peripheral to use
 * @param reg [uint8_t] Register Address
 * @param data [uint8_t] Data to write
 * @return SPI_Status
 */
SPI_Status SPI_Write_Reg(SPI_TypeDef* SPI, uint8_t reg, uint8_t data);

/**
 * @brief Read Data over SPI
 * @note Must toggle CS pin manually
 * @note Assumes 8-bit registers
 * 
 * @param SPI [SPI_TypeDef*] SPI Peripheral to use
 * @param reg [uint8_t] Register Address
 * @return uint8_t Register Data
 */
uint8_t SPI_Read_Reg(SPI_TypeDef* SPI, uint8_t reg);

/**
 * @brief Write a data buffer to the SPI Bus
 * 
 * @param SPI [SPI_TypeDef*] SPI Peripheral to use
 * @param reg [uint8_t] Register Address
 * @param data [uint8_t*] Data Buffer to write
 * @param len [size_t] Length of Data Buffer
 * @return SPI_Status 
 */
SPI_Status SPI_Write(SPI_TypeDef* SPI, uint8_t reg, uint8_t* data, size_t len);

/**
 * @brief Read Data from the SPI Bus
 * 
 * @param SPI [SPI_TypeDef*] SPI Peripheral to use
 * @param reg [uint8_t] Register Address
 * @param data [uint8_t*] Data Buffer to read into
 * @param len [size_t] Length of Data Buffer
 * @return SPI_Status 
 */
SPI_Status SPI_Read(SPI_TypeDef* SPI, uint8_t reg, uint8_t* data, size_t len);