/************************************************
* @file    lora.c
* @author  APBashara
* @date    10/2024
* 
* @brief   Implementation of RFM95W LoRa Module Driver
***********************************************/                                                                                                                                               

#include "lora.h"

/* Function Implementation --------------------------------------------------*/
LoRa_Status Lora_Init() {
    // Set RFM95W to LoRa Mode
    uint8_t regData = SPI_Read_Register(SPI2, RegOpMode, LORA_CS);
    regData |= RegOpMode_LongRangeMode; 
    SPI_Write_Register(SPI2, RegOpMode, regData, LORA_CS);
    
    // Calculate and set Carrier Frequency
    uint32_t F = (LORA_FREQ * 524288) >> 5;
    regData = (F >> 16); // MSB
    SPI_Write_Register(SPI2, RegFrfMsb, regData, LORA_CS);
    regData = (F >> 8); // MID
    SPI_Write_Register(SPI2, RegFrfMid, regData, LORA_CS);
    regData = (F >> 0); // LSB
    SPI_Write_Register(SPI2, RegFrfLsb, regData, LORA_CS);

    // Set Power to 20 dBm
    SPI_Write_Register(SPI2, RegPaConfig, RegPaConfig_20dBm, LORA_CS);

    // Set Bandwidth to 500 kHz and Coding Rate to 4/5
    regData = SPI_Read_Register(SPI2, RegModemConfig1, LORA_CS);
    regData |= (0x9 << RegModemConfig1_Bw_Pos) | (0x1 << RegModemConfig1_CodingRate_Pos);
    SPI_Write_Register(SPI2, RegModemConfig1, regData, LORA_CS);

    // Set Spreading Factor to 6
    regData = SPI_Read_Register(SPI2, RegModemConfig2, LORA_CS);
    regData |= (0x6 << RegModemConfig2_SpreadingFactor_Pos);
    SPI_Write_Register(SPI2, RegModemConfig1, regData, LORA_CS);

    return LORA_OK;
}

