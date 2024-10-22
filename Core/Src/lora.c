/************************************************
* @file    lora.c
* @author  APBashara
* @date    10/2024
* 
* @brief   Implementation of RFM95W LoRa Module Driver
***********************************************/                                                                                                                                               

#include "lora.h"

/* Function Implementation --------------------------------------------------*/
void Lora_Init() {
    // Set RFM95W to LoRa Mode
    SPI_Write_Register(SPI2, RegOpMode, 0x80, 0);
    // Set Frequency to 915 MHz
    SPI_Write_Register(SPI2, RegFrfMsb, 0xE4, 0);
    SPI_Write_Register(SPI2, RegFrfMid, 0xC0, 0);
    SPI_Write_Register(SPI2, RegFrfLsb, 0x00, 0);
    // Set Power to 20 dBm
    SPI_Write_Register(SPI2, RegPaConfig, 0xFF, 0);
    SPI_Write_Register(SPI2, RegPaDac, 0x87, 0);
    // Set Bandwidth to 500 kHz
    SPI_Write_Register(SPI2, RegModulationCfg, 0x72, 0);
    // Set Coding Rate to 4/5
    SPI_Write_Register(SPI2, RegModemStat, 0x72, 0);
    // Set Spreading Factor to 12
    SPI_Write_Register(SPI2, RegModemStat, 0x72, 0);
}