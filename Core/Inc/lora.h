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
#define LORA_FREQ           915000000 // 915MHz
#define RFM95_OSC_FREQ      32000000 // 32MHz
#define LORA_SPI            (SPI_TypeDef*)SPI2
#define LORA_GPIO           (GPIO_TypeDef*)GPIOB
#define LORA_CS             12 // PB12 for CS
#define LORA_WRITE          0x80 // Mask for write command

#define LORA_IO_PORT        (GPIO_TypeDef*)GPIOA
#define LORA_RST            8 // PA8 for Reset
#define LORA_INT            9 // PA9 for Interrupt

#define LORA_RETRY          5 // Number of Retries for LoRa Operations

/* Structs and Enums --------------------------------------------------------*/
typedef enum {
    LORA_OK,
    LORA_ERROR,
} LoRa_Status;

typedef enum {
    LORA_SLEEP,
    LORA_STANDBY,
    LORA_FREQ_SYNTH_TX,
    LORA_TX,
    LORA_FREQ_SYNTH_RX,
    LORA_RX_CONTINUOUS,
    LORA_RX_SINGLE,
    LORA_CAD,
} LoRa_Mode;

typedef enum {
    LORA_SF_6 = 6,
    LORA_SF_7 = 7,
    LORA_SF_8 = 8,
    LORA_SF_9 = 9,
    LORA_SF_10 = 10,
    LORA_SF_11 = 11,
    LORA_SF_12 = 12,
} LoRa_SF;

typedef enum {
    LORA_BW_7_8,
    LORA_BW_10_4,
    LORA_BW_15_6,
    LORA_BW_20_8,
    LORA_BW_31_25,
    LORA_BW_41_7,
    LORA_BW_62_5,
    LORA_BW_125,
    LORA_BW_250,
    LORA_BW_500,
} LoRa_BW;

typedef enum {
    LORA_CR_ERROR,
    LORA_CR_4_5,
    LORA_CR_4_6,
    LORA_CR_4_7,
    LORA_CR_4_8,
} LoRa_CR;

/* Function Prototypes ------------------------------------------------------*/

/**
 * @brief Initialize RFM95w on SPI2
 * @note default 915 MHz, 20 dBm, 500 kHz Bandwidth, 4/5 Coding Rate
 */
LoRa_Status Lora_Init();

/**
 * @brief Set the Spreading Factor for LoRa
 * @note If SF is out of range, it will be set to the closest value
 * 
 * @param sf [uint8_t] Spreading Factor from 6-12
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