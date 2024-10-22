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
    // Set RFM95W to LoRa Mode
    SPI_Write_Register(SPI2, RegOpMode, 0x80, 1, 12);
    // Set Frequency to 915 MHz
    SPI_Write_Register(SPI2, RegFrfMsb, 0xE4, 1, 12);
    SPI_Write_Register(SPI2, RegFrfMid, 0xC0, 1, 12);
    SPI_Write_Register(SPI2, RegFrfLsb, 0x00, 1, 12);
    // Set Power to 20 dBm
    SPI_Write_Register(SPI2, RegPaConfig, 0xFF, 1, 12);
    SPI_Write_Register(SPI2, RegPaDac, 0x87, 1, 12);
    // Set Bandwidth to 500 kHz
    SPI_Write_Register(SPI2, RegModulationCfg, 0x72, 1, 12);
    // Set Coding Rate to 4/5
    SPI_Write_Register(SPI2, RegModemStat, 0x72, 1, 12);
    // Set Spreading Factor to 12
    SPI_Write_Register(SPI2, RegModemStat, 0x72, 1, 12);

    return LORA_OK;
}

