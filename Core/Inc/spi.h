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
 * @note Written to be used with RFM95W
 */
void SPI2_Init();

/**
 * @brief Transmit data over SPI
 * @note Blocking/Polling Call
 * 
 * @param SPI [SPI_TypeDef*] SPI Peripheral to use
 * @param data [uint8_t*] Data buffer to send
 * @param len [size_t] Length of data buffer in bytes
 * @return SPI_Status 
 */
SPI_Status SPI_Transmit(SPI_TypeDef* SPI, uint8_t* data, size_t len);

/**
 * @brief Receive data over SPI
 * @note Blocking/Polling Call
 * 
 * @param SPI [SPI_TypeDef*] SPI Peripheral to use
 * @param buf [uint8_t*] Buffer to read into
 * @param len [size_t] Length of buffer in bytes
 * @return SPI_Status 
 */
SPI_Status SPI_Receive(SPI_TypeDef* SPI, uint8_t* buf, size_t len);