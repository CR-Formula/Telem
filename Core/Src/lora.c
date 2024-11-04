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

static LoRa_Status Lora_Write_Reg(uint8_t reg, uint8_t data) {
    uint8_t txData[2] = {reg | LORA_WRITE, data};
    Clear_Pin(LORA_GPIO, LORA_CS);
    SPI_Transmit(LORA_SPI, txData, sizeof(txData));
    Set_Pin(LORA_GPIO, LORA_CS);
    return LORA_OK;
}

static LoRa_Status Lora_Write(uint8_t reg, uint8_t* data, size_t len) {
    return LORA_OK;
}

static uint8_t Lora_Read_Reg(uint8_t reg) {
    Clear_Pin(LORA_GPIO, LORA_CS);
    SPI_Transmit(LORA_SPI, &reg, 1);
    uint8_t data;
    SPI_Receive(LORA_SPI, &data, 1);
    Set_Pin(LORA_GPIO, LORA_CS);
    return data;
}

static LoRa_Status Lora_Read(uint8_t reg, uint8_t* data, size_t len) {
    return LORA_OK;
}

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

LoRa_Status Lora_Transmit(uint8_t* data, uint8_t len) {
    return LORA_OK;
}

LoRa_Status Lora_Receive(uint8_t* data, uint8_t len) {
    return LORA_OK;
}

LoRa_Status Lora_Set_SF(uint8_t sf) {
    return LORA_OK;
}

LoRa_Status Lora_Set_BW(uint8_t bw) {
    return LORA_OK;
}

LoRa_Status Lora_Set_Power(uint8_t power) {
    return LORA_OK;
}

LoRa_Status Lora_Set_CodingRate(uint8_t cr) {
    return LORA_OK;
}