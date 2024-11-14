/************************************************
* @file    lora.c
* @author  APBashara
* @date    10/2024
* 
* @brief   Implementation of RFM95W LoRa Module Driver
***********************************************/                                                                                                                                               

#include "lora.h"

LoRa_Mode loraMode = LORA_STANDBY; // Module inits to Standby Mode

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

/** 
 * @brief Set the Mode of the LoRa Module
 * @note Also updates the global loraMode variable
 * 
 * @param mode [LoRa_Mode] Mode to set the module to
 * @return LoRa_Status
 */
static LoRa_Status Lora_Set_Mode(LoRa_Mode mode);

/* Function Implementation --------------------------------------------------*/

static LoRa_Status Lora_Write_Reg(uint8_t reg, uint8_t data) {
    uint8_t txData[2] = {reg | LORA_WRITE, data};
    Clear_Pin(LORA_GPIO, LORA_CS);
    SPI_Transmit(LORA_SPI, txData, sizeof(txData));
    Set_Pin(LORA_GPIO, LORA_CS);
    return LORA_OK;
}

static LoRa_Status Lora_Write(uint8_t reg, uint8_t* data, size_t len) {
    reg = reg | LORA_WRITE;
    Clear_Pin(LORA_GPIO, LORA_CS);
    SPI_Transmit(LORA_SPI, &reg, 1);
    SPI_Transmit(LORA_SPI, data, len);
    Set_Pin(LORA_GPIO, LORA_CS);
    return LORA_OK;
}

static uint8_t Lora_Read_Reg(uint8_t reg) {
    uint8_t data;
    reg &= ~LORA_WRITE;
    Clear_Pin(LORA_GPIO, LORA_CS);
    SPI_Transmit(LORA_SPI, &reg, 1);
    SPI_Receive(LORA_SPI, &data, 1);
    Set_Pin(LORA_GPIO, LORA_CS);
    return data;
}

static LoRa_Status Lora_Read(uint8_t reg, uint8_t* data, size_t len) {
    Clear_Pin(LORA_GPIO, LORA_CS);
    SPI_Transmit(LORA_SPI, &reg, 1);
    SPI_Receive(LORA_SPI, data, len);
    Set_Pin(LORA_GPIO, LORA_CS);
    return LORA_OK;
}

static LoRa_Status Lora_Set_Mode(LoRa_Mode mode) {
    uint8_t regData = Lora_Read_Reg(RegOpMode);
    regData &= ~RegOpMode_Mode;
    regData |= (mode << RegOpMode_Mode_Pos);
    Lora_Write_Reg(RegOpMode, regData);
    loraMode = Lora_Read_Reg(RegOpMode) & RegOpMode_Mode;
    if (loraMode != mode) {
        return LORA_ERROR;
    }
    return LORA_OK;
}

LoRa_Status Lora_Init() {
    volatile uint8_t regData = 0;
    // Enter Sleep Mode
    Lora_Set_Mode(LORA_SLEEP);

    // Set Long Range Mode (LoRa)
    regData = Lora_Read_Reg(RegOpMode);
    regData |= RegOpMode_LongRangeMode;
    Lora_Write_Reg(RegOpMode, regData);
    if (!(Lora_Read_Reg(RegOpMode) & RegOpMode_LongRangeMode)) {
        return LORA_ERROR;
    }

    // Set Access to HF Test Registers
    regData = Lora_Read_Reg(RegOpMode);
    regData &= ~RegOpMode_LowFrequencyModeOn;
    Lora_Write_Reg(RegOpMode, regData);
    
    // Calculate and set Carrier Frequency
    uint32_t FreqStep = RFM95_OSC_FREQ / 524288;
    uint32_t F = LORA_FREQ / FreqStep;
    regData = ((F >> 16) & 0xFF); // MSB
    Lora_Write_Reg(RegFrfMsb, regData);
    regData = ((F >> 8) & 0xFF); // MID
    Lora_Write_Reg(RegFrfMid, regData);
    regData = ((F >> 0) & 0xFF); // LSB
    Lora_Write_Reg(RegFrfLsb, regData);

    // Set Power to 20 dBm
    Lora_Write_Reg(RegPaConfig, RegPaConfig_20dBm);
    Lora_Write_Reg(RegPaDac, RegPaDac_20dBm);

    // Defaults to 4/5 Coding Rate, 500 kHz Bandwidth, SF 6
    Lora_Set_CodingRate(LORA_CR_4_5);
    Lora_Set_BW(LORA_BW_500);
    Lora_Set_SF(LORA_SF_6);

    // Set the Module to Standby Mode
    Lora_Set_Mode(LORA_STANDBY);

    return LORA_OK;
}

LoRa_Status Lora_Set_SF(uint8_t sf) {
    // Check input range
    if (sf < LORA_SF_6) {
        sf = LORA_SF_6;
    }
    else if (sf > LORA_SF_12) {
        sf = LORA_SF_12;
    }

    // Read and Rewrite Register
    uint8_t regData = Lora_Read_Reg(RegModemConfig2);
    regData &= ~RegModemConfig2_SpreadingFactor;
    regData |= (sf << RegModemConfig2_SpreadingFactor_Pos);
    Lora_Write_Reg(RegModemConfig2, regData);

    return LORA_OK;
}

LoRa_Status Lora_Set_BW(uint8_t bw) {
    // Check input range
    if (bw < LORA_BW_7_8) {
        bw = LORA_BW_7_8;
    }
    else if (bw > LORA_BW_500) {
        bw = LORA_BW_500;
    }

    // Read and Rewrite Register
    uint8_t regData = Lora_Read_Reg(RegModemConfig1);
    regData &= ~RegModemConfig1_Bw;
    regData |= (bw << RegModemConfig1_Bw_Pos);
    Lora_Write_Reg(RegModemConfig1, regData);
    return LORA_OK;
}

LoRa_Status Lora_Set_CodingRate(uint8_t cr) {
    // Check input range
    if (cr < LORA_CR_4_5) {
        cr = LORA_CR_4_5;
    }
    else if (cr > LORA_CR_4_8) {
        cr = LORA_CR_4_8;
    }

    // Read and Rewrite Register
    uint8_t regData = Lora_Read_Reg(RegModemConfig1);
    regData &= ~RegModemConfig1_CodingRate;
    regData |= (cr << RegModemConfig1_CodingRate_Pos);
    Lora_Write_Reg(RegModemConfig1, regData);
    return LORA_OK;
}

LoRa_Status Lora_Transmit(uint8_t* data, uint8_t len) {
    if (len == 0 || len == NULL || len > LORA_MAX_PAYLOAD_LEN || data == NULL) {
        return LORA_ERROR;
    }

    uint8_t regData = 0;

    if (loraMode != LORA_STANDBY) {
        // Set to Standby Mode
        regData = Lora_Read_Reg(RegOpMode);
        regData &= (LORA_STANDBY << RegOpMode_Mode_Pos);
        Lora_Write_Reg(RegOpMode, regData);
        loraMode = LORA_STANDBY;
    }

    // Set Address Pointer to FIFO
    // regData = Lora_Read_Reg(RegFifoTxBaseAddr);
    Lora_Write_Reg(RegFifoAddrPtr, RegFifo);

    // Write data to FIFO
    Lora_Write(RegFifo, data, len);

    // Set Payload Length
    Lora_Write_Reg(RegPayloadLength, len);

    // Set to TX Mode
    Lora_Set_Mode(LORA_TX);

    // Check for TX Done
    while(1) {
        regData = Lora_Read_Reg(RegIrqFlags);
        if (regData & RegIrqFlags_TxDone) {
            break;
        }
    }

    return LORA_OK;
}

LoRa_Status Lora_Receive(uint8_t* data, uint8_t len) {
    return LORA_OK;
}