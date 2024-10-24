/************************************************
* @file    lora.c
* @author  APBashara
* @date    10/2024
* 
* @brief   Implementation of RFM95W LoRa Module Driver
***********************************************/                                                                                                                                               

#include "lora.h"

LoRa_Status Lora_Write(uint8_t reg, uint8_t data) {
    reg = reg | LORA_WRITE;
    SPI_Write_Register(SPI2, reg, data, LORA_CS);
    return LORA_OK;
}

LoRa_Status LoRa_Read(uint8_t reg, uint8_t* data) {
    // TODO: fix SPI_Read_Register to implement
}

/* Function Implementation --------------------------------------------------*/
LoRa_Status Lora_Init() {
    // Set RFM95W to LoRa Mode
    uint8_t regData = SPI_Read_Register(SPI2, RegOpMode, LORA_CS);
    regData |= RegOpMode_LongRangeMode;
    Lora_Write(RegOpMode, regData);
    
    // Calculate and set Carrier Frequency
    uint32_t F = (LORA_FREQ * 524288) >> 5;
    regData = (F >> 16); // MSB
    Lora_Write(RegFrfMsb, regData);
    regData = (F >> 8); // MID
    Lora_Write(RegFrfMid, regData);
    regData = (F >> 0); // LSB
    Lora_Write(RegFrfLsb, regData);

    // Set Power to 20 dBm
    Lora_Write(RegPaConfig, RegPaConfig_20dBm);

    // Set Bandwidth to 500 kHz and Coding Rate to 4/5
    regData = SPI_Read_Register(SPI2, RegModemConfig1, LORA_CS);
    regData |= (0x9 << RegModemConfig1_Bw_Pos) | (0x1 << RegModemConfig1_CodingRate_Pos);
    SPI_Write_Register(SPI2, RegModemConfig1, regData, LORA_CS);

    // Set Spreading Factor to 6
    regData = SPI_Read_Register(SPI2, RegModemConfig2, LORA_CS);
    regData |= (0x6 << RegModemConfig2_SpreadingFactor_Pos);
    Lora_Write(RegModemConfig2, regData);

    return LORA_OK;
}

