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

#define LORA_FREQ 915000000 // 915MHz


/* Function Prototypes ------------------------------------------------------*/

/**
 * @brief Initialize RFM95w on SPI2
 * @note default 915 MHz, 20 dBm, 500 kHz Bandwidth, 4/5 Coding Rate
 * 
 */
void Lora_Init();

/**
 * @brief Send data over LoRa
 * 
 * @param data [uint8_t*] Data buffer to send
 * @param len [uint8_t] Length of data
 */
void Lora_Send(uint8_t* data, uint8_t len);

/**
 * @brief Read data from LoRa
 * 
 * @param data [uint8_t*] Data buffer to read into
 * @param len [uint8_t] Length of data
 */
void Lora_Receive(uint8_t* data, uint8_t len);

/**
 * @brief Set the Spreading Factor for LoRa
 * 
 * @param sf [uint8_t] Spreading Factor from 1-12
 */
void Lora_Set_SF(uint8_t sf);

/**
 * @brief Set the Bandwidth for LoRa
 * 
 * @param bw [uint8_t] Bandwidth in kHz
 */
void Lora_Set_BW(uint8_t bw);

