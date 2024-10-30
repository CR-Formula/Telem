/************************************************
* @file    lora.h 
* @author  APBashara
* @date    10/2024
* 
* @brief   Prototype Functions for RFM95W Device Driver
***********************************************/

#include "stm32f415xx.h"
#include "spi.h"
#include "rfm95_reg.h"
#include "gpio.h"

/* Macros -------------------------------------------------------------------*/
#define LORA_FREQ           915 // MHz
#define LORA_SPI            (SPI_TypeDef*)SPI2
#define LORA_GPIO           (GPIO_TypeDef*)GPIOB
#define LORA_CS             12 // PB12 for CS
#define LORA_WRITE          0x80 // Mask for write command

/* Structs and Enums --------------------------------------------------------*/
typedef enum {
    LORA_OK,
    LORA_ERROR,
} LoRa_Status;

/* Function Prototypes ------------------------------------------------------*/

/**
 * @brief Initialize RFM95w on SPI2
 * @note default 915 MHz, 20 dBm, 500 kHz Bandwidth, 4/5 Coding Rate
 */
LoRa_Status Lora_Init();

/**
 * @brief Send data buffer over LoRa Connection
 * 
 * @param data [uint8_t*] Data buffer to send
 * @param len [uint8_t] Length of data
 */
LoRa_Status Lora_Transmit(uint8_t* data, uint8_t len);

/**
 * @brief Read data from LoRa Connection
 * 
 * @param data [uint8_t*] Data buffer to read into
 * @param len [uint8_t] Length of data
 * @return LoRa_Status
 */
LoRa_Status Lora_Receive(uint8_t* data, uint8_t len);

/**
 * @brief Set the Spreading Factor for LoRa
 * 
 * @param sf [uint8_t] Spreading Factor from 1-12
 * @return LoRa_Status
 */
LoRa_Status Lora_Set_SF(uint8_t sf);

/**
 * @brief Set the Bandwidth for LoRa
 * 
 * @param bw [uint8_t] Bandwidth in kHz
 * @return LoRa_Status
 */
LoRa_Status Lora_Set_BW(uint8_t bw);

/**
 * @brief Set the TX Power for LoRa
 * 
 * @param power [uint8_t] Power in dBm
 * @return LoRa_Status 
 */
LoRa_Status Lora_Set_Power(uint8_t power);

/**
 * @brief Set the Coding Rate for LoRa
 * 
 * @param cr [uint8_t] Coding Rate
 * @return LoRa_Status 
 */
LoRa_Status Lora_Set_CodingRate(uint8_t cr);