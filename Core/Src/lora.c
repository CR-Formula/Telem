/************************************************
* @file    lora.c
* @author  APBashara
* @date    10/2024
* 
* @brief   Implementation of RFM95W LoRa Module Driver
***********************************************/                                                                                                                                               

#include "lora.h"

LoRa_Mode loraMode = LORA_STANDBY; // Module inits to Standby Mode
uint8_t loraRecvFlag = 0;

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
    reg |= LORA_WRITE;
    Clear_Pin(LORA_GPIO, LORA_CS);
    SPI_Transmit(LORA_SPI, &reg, 1);
    SPI_Transmit(LORA_SPI, &data, 1);
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
    // Rising Edge Interrupt on PA9
    // Calls EXTI9_5_IRQHandler when PA9 gets set high
    GPIO_EXTI_Init(LORA_GPIO, LORA_INT);
    NVIC_SetPriority(EXTI9_5_IRQn, 2);
    NVIC_EnableIRQ(EXTI9_5_IRQn);

    // Enter Sleep Mode
    Lora_Set_Mode(LORA_SLEEP);

    // Set Long Range Mode (LoRa) and High Frequency Mode
    regData = Lora_Read_Reg(RegOpMode);
    regData |= RegOpMode_LongRangeMode;
    regData &= ~RegOpMode_LowFrequencyModeOn;
    Lora_Write_Reg(RegOpMode, regData);
    if (!(Lora_Read_Reg(RegOpMode) & RegOpMode_LongRangeMode) ||
        (Lora_Read_Reg(RegOpMode) & RegOpMode_LowFrequencyModeOn)) {
        return LORA_ERROR;
    }

    // Configure FIFO Pointers
    Lora_Write_Reg(RegFifoTxBaseAddr, RegFifo);
    Lora_Write_Reg(RegFifoRxBaseAddr, RegFifo);
    if (Lora_Read_Reg(RegFifoTxBaseAddr) != RegFifo ||
        Lora_Read_Reg(RegFifoRxBaseAddr) != RegFifo) {
        return LORA_ERROR;
    }

    // Set the Module to Standby Mode and wait for it to enter
    while (Lora_Set_Mode(LORA_STANDBY) != LORA_OK);

    // Set Bandwidth and Coding Rate
    Lora_Set_CodingRate(LORA_CR_4_5);
    Lora_Set_BW(LORA_BW_500);

    // Implicit Header Mode
    regData = Lora_Read_Reg(RegModemConfig1);
    regData |= RegModemConfig1_ImplicitHeaderModeOn;
    Lora_Write_Reg(RegModemConfig1, regData);

    // Set Spreading Factor, CRC
    Lora_Set_SF(LORA_SF_7);
    Lora_Set_CRC(false);

    regData = Lora_Read_Reg(RegModemConfig2);

    // Disable Low data rate and set AGC On
    regData = Lora_Read_Reg(RegModemConfig3);
    regData &= ~RegModemConfig3_LowDataRateOpt;
    regData |= RegModemConfig3_AgcAutoOn;
    Lora_Write_Reg(RegModemConfig3, regData);

    // Set Preamble Length
    Lora_Set_Preamble(8);
    
    // Calculate and set Carrier Frequency
    // To use a different frequency, change the LORA_FREQ macro
    regData = (((uint32_t)RFM95_Frf >> 16) & 0xFF); // MSB
    Lora_Write_Reg(RegFrfMsb, regData);
    regData = (((uint32_t)RFM95_Frf >> 8) & 0xFF); // MID
    Lora_Write_Reg(RegFrfMid, regData);
    regData = (((uint32_t)RFM95_Frf >> 0) & 0xFF); // LSB
    Lora_Write_Reg(RegFrfLsb, regData);

    // Set Power to 20 dBm
    Lora_Write_Reg(RegPaDac, RegPaDac_20dBm);
    Lora_Write_Reg(RegPaConfig, RegPaConfig_20dBm);

    return LORA_OK;
}

LoRa_Status Lora_Set_SF(LoRa_SF sf) {
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

LoRa_Status Lora_Set_BW(LoRa_BW bw) {
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

LoRa_Status Lora_Set_CodingRate(LoRa_CR cr) {
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

LoRa_Status Lora_Set_CRC(bool crc) {
    uint8_t regData = Lora_Read_Reg(RegModemConfig2);
    if (crc) {
        regData |= RegModemConfig2_RxPayloadCrcOn;
    }
    else {
        regData &= ~RegModemConfig2_RxPayloadCrcOn;
    }
    Lora_Write_Reg(RegModemConfig2, regData);
    
    return LORA_OK;
}

LoRa_Status Lora_Set_Preamble(uint16_t preamble) {
    Lora_Write_Reg(RegPreambleMsb, ((preamble >> 8) & 0xFF));
    Lora_Write_Reg(RegPreambleLsb, (preamble & 0xFF));
    return LORA_OK;
}

LoRa_Status Lora_Transmit(uint8_t* data, size_t len) {
    if (len == 0 || len == NULL || len > LORA_MAX_PAYLOAD_LEN || data == NULL) {
        return LORA_ERROR;
    }

    uint8_t regData = 0;

    Lora_Set_Mode(LORA_STANDBY);

    // Set Address Pointer to FIFO
    Lora_Write_Reg(RegFifoAddrPtr, RegFifo);

    // Write data to FIFO
    Lora_Write(RegFifo, data, len);

    // Set Payload Length
    Lora_Write_Reg(RegPayloadLength, len);

    // Clear TXDone and Set to TX Mode
    Lora_Write_Reg(RegIrqFlags, RegIrqFlags_TxDone);
    Lora_Set_Mode(LORA_TX);

    // Check for TX Done
    // TODO: impelment IRQ for TX_Done signal
    // Can use hardware pin or check register
    while (1) {
        regData = Lora_Read_Reg(RegIrqFlags);
        if (regData & RegIrqFlags_TxDone) {
            break;
        }
    }

    return LORA_OK;
}

LoRa_Status Lora_Receive(uint8_t* data, uint8_t* len) {
    volatile uint8_t regData = 0;
    volatile uint8_t opMode = 0;

    // Set to RX Continuous Mode
    Lora_Set_Mode(LORA_STANDBY);
    Lora_Set_Mode(LORA_RX_CONTINUOUS);
    opMode = Lora_Read_Reg(RegOpMode);
    
    // if (Lora_Set_Mode(LORA_RX_CONTINUOUS) != LORA_OK) {
    //     return LORA_ERROR;
    // }

    // Wait for RX Done
    while (1) {
        regData = Lora_Read_Reg(RegIrqFlags);
        if (regData & RegIrqFlags_RxDone) {
            break;
        }
    }

    // Read the length of the received packet
    *len = Lora_Read_Reg(RegRxNbBytes);

    // Set FIFO address to current RX address
    Lora_Write_Reg(RegFifoAddrPtr, Lora_Read_Reg(RegFifoRxCurrentAddr));

    // Read data from FIFO
    Lora_Read(RegFifo, data, 13); // TODO: address the magic number

    // Clear RX Done flag
    Lora_Write_Reg(RegIrqFlags, RegIrqFlags_RxDone);

    return LORA_OK;
}