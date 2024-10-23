/************************************************
* @file    lora.c
* @author  APBashara
* @date    10/2024
* 
* @brief   Implementation of RFM95W LoRa Module Driver
***********************************************/                                                                                                                                               

#include "lora.h"

/* Function Implementation --------------------------------------------------*/
Lora_Status Lora_Init() {
    uint8_t regData = 0x0;
    // Set RFM95W to LoRa Modeardu
    regData |= RegOpMode_LongRangeMode; 
    SPI_Write_Register(SPI2, RegOpMode, regData, LORA_CS);
    
    // Calculate and set Carrier Frequency
    uint32_t F = (LORA_FREQ * 524288) >> 5;
    // MSB
    regData = (F >> 16);
    SPI_Write_Register(SPI2, RegFrfMsb, regData, LORA_CS);
    // MID
    regData = (F >> 8);
    SPI_Write_Register(SPI2, RegFrfMid, regData, LORA_CS);
    // LSB
    regData = (F >> 0);
    SPI_Write_Register(SPI2, RegFrfLsb, regData, LORA_CS);

    // Set Power to 20 dBm
    SPI_Write_Register(SPI2, RegPaConfig, RegPaConfig_20dBm, LORA_CS);
    // Set Bandwidth to 500 kHz
    SPI_Write_Register(SPI2, RegModulationCfg, 0x72, LORA_CS);
    // Set Coding Rate to 4/5
    SPI_Write_Register(SPI2, RegModemStat, 0x72, LORA_CS);
    // Set Spreading Factor to 12
    SPI_Write_Register(SPI2, RegModemStat, 0x72, LORA_CS);

    return LORA_OK;
}

