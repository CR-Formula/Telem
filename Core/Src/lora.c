/************************************************
* @file    lora.c
* @author  APBashara
* @date    10/2024
* 
* @brief   Implementation of RFM95W LoRa Module Driver
***********************************************/                                                                                                                                               

#include "lora.h"

/* Static Functions ---------------------------------------------------------*/

/**
 * @brief Send a single byte to a register on the LoRa Module
 * 
 * @param reg [uint8_t] Register Address
 * @param data [uint8_t] Data to write
 * @return LoRa_Status
 */
static LoRa_Status Lora_Write_Reg(uint8_t reg, uint8_t data);

/**
 * @brief Send multiple bytes over SPI to the LoRa Module
 * 
 * @param reg [uint8_t] Register Address
 * @param data [uint8_t*] Data buffer to send
 * @param len [size_t] Length of data buffer
 * @return LoRa_Status 
 */
static LoRa_Status Lora_Write(uint8_t reg, uint8_t* data, size_t len);

/**
 * @brief Read a single byte from the LoRa Module
 * 
 * @param reg [uint8_t] Register Address
 * @return uint8_t data read
 */
static uint8_t Lora_Read_Reg(uint8_t reg);

/**
 * @brief Read multiple bytes from the LoRa Module
 * 
 * @param reg [uint8_t] Register Address
 * @param data [uint8_t*] Data buffer to read into
 * @param len [size_t] Length of data buffer
 * @return LoRa_Status 
 */
static LoRa_Status Lora_Read(uint8_t reg, uint8_t* data, size_t len);

/* Function Implementation --------------------------------------------------*/

LoRa_Status Lora_Init() {
    // Set RFM95W to LoRa Mode
    uint8_t regData = Lora_Read_Reg(RegOpMode);
    regData |= RegOpMode_LongRangeMode;
    Lora_Write_Reg(RegOpMode, regData);
    
    // Calculate and set Carrier Frequency
    uint32_t F = (LORA_FREQ * 524288) >> 5;
    regData = (F >> 16); // MSB
    Lora_Write_Reg(RegFrfMsb, regData);
    regData = (F >> 8); // MID
    Lora_Write_Reg(RegFrfMid, regData);
    regData = (F >> 0); // LSB
    Lora_Write_Reg(RegFrfLsb, regData);

    // Set Power to 20 dBm
    Lora_Write_Reg(RegPaConfig, RegPaConfig_20dBm);

    // Set Bandwidth to 500 kHz and Coding Rate to 4/5
    regData = Lora_Read_Reg(RegModemConfig1);
    regData |= (0x9 << RegModemConfig1_Bw_Pos) | (0x1 << RegModemConfig1_CodingRate_Pos);
    Lora_Write_Reg(RegModemConfig1, regData);

    // Set Spreading Factor to 6
    regData = Lora_Read_Reg(RegModemConfig2);
    regData |= (0x6 << RegModemConfig2_SpreadingFactor_Pos);
    Lora_Write_Reg(RegModemConfig2, regData);

    return LORA_OK;
}

static LoRa_Status Lora_Write_Reg(uint8_t reg, uint8_t data) {
    reg = reg | LORA_WRITE; // Set Write Bit
    Clear_Pin(LORA_GPIO, LORA_CS); // Set CS Low
    SPI_Transmit(LORA_SPI, reg); // Send Address
    SPI_Transmit(LORA_SPI, data); // Send Data
    Set_Pin(LORA_GPIO, LORA_CS); // Set CS High
    return LORA_OK;
}

static LoRa_Status Lora_Write(uint8_t reg, uint8_t* data, size_t len) {
    reg = reg | LORA_WRITE; // Set Write Bit
    Clear_Pin(LORA_GPIO, LORA_CS); // Set CS Low
    SPI_Transmit(LORA_SPI, reg); // Send Address
    for (size_t i = 0; i < len; i++) {
        SPI_Transmit(LORA_SPI, data[i]); // Send Data
    }
    Set_Pin(LORA_GPIO, LORA_CS); // Set CS High
    return LORA_OK;
}

static uint8_t Lora_Read_Reg(uint8_t reg) {
    reg = reg & ~LORA_WRITE; // Clear Read Bit
    Clear_Pin(LORA_GPIO, LORA_CS); // Set CS Low
    SPI_Transmit(LORA_SPI, reg); // Send Address
    uint8_t data = SPI_Transmit(LORA_SPI, 0); // Read Data
    Set_Pin(LORA_GPIO, LORA_CS); // Set CS High
    return data;
}

static LoRa_Status Lora_Read(uint8_t reg, uint8_t* data, size_t len) {
    reg = reg & ~LORA_WRITE; // Clear Read Bit
    SPI_Transmit(LORA_SPI, reg); // Send Address
    for (size_t i = 0; i < len; i++) {
        data[i] = SPI_Transmit(LORA_SPI, 0); // Read Data
    }
    return LORA_OK;
}

