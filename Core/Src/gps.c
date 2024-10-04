/************************************************
* @file    gps.c 
* @author  APBashara
* @date    9/2024
* 
* @brief   Neo-M9N GPS Driver Implementation
***********************************************/

#include "gps.h"


/* Static Functions ---------------------------------------------------------*/
static GPS_Status checkACK(uint8_t* response, size_t msg_len, uint8_t class, uint8_t id) {
    if (response[UBX_ACK_CLASS] == class && 
        response[UBX_ACK_ID] == id) {
        return GPS_OK;
    } else {
        return GPS_ERROR;
    }
}

static GPS_Status calcChecksum(uint8_t* msg, size_t msg_len, uint8_t* CK_A, uint8_t* CK_B) {
    for (size_t i = 0; i < msg_len; i++) {
        *CK_A += msg[i];
        *CK_B += *CK_A;
    }
    return GPS_OK;
}

static size_t getAvailableBytes(I2C_TypeDef* I2C, uint8_t dev, size_t* len) {
    *len = 0;
    uint8_t data[2];
    // I2C_Read(I2C, dev, data, 2);
    data[0] = I2C_Read_Reg(I2C, dev, M9N_MSB_REG);
    data[1] = I2C_Read_Reg(I2C, dev, M9N_LSB_REG);
    *len = data[0] << 8 | data[1];
    return GPS_OK;
}


/* Function Implementation --------------------------------------------------*/
GPS_Status GPS_Init() {
    uint8_t ubx_msg[] = {
        0xB5, 0x62, // Sync Chars
        0x06, 0x8A, // Class (CFG), ID (VALSET)
        0x0C, 0x00, // Length of payload (12 bytes)
        0x00,       // Version (0x00)
        0x01,       // Layer (0x01 for RAM, or 0x04 for Flash)
        0x00,       // Transaction (0x00, no transaction)
        0x00,       // Reserved (always 0)
        // Start of Key/Value pairs
        0x01, 0x00, 0x21, 0x30, // CFG-RATE-MEAS Key (0x30210001 in little-endian)
        // Value for CFG-RATE-MEAS (40 ms for 25 Hz)
        0x28, 0x00, 0x00, 0x00, // 40 ms (in little-endian)
        0x00, 0x00  // Checksum placeholder
    };

    // Calculate checksum for the message
    uint8_t checksumA = 0;
    uint8_t checksumB = 0;
    
    // Checksum starts after sync chars (index 2), and includes Class, ID, Length, and Payload
    for (int i = 2; i < sizeof(ubx_msg) - 2; i++) {
        checksumA += ubx_msg[i];
        checksumB += checksumA;
    }

    ubx_msg[sizeof(ubx_msg) - 2] = checksumA;
    ubx_msg[sizeof(ubx_msg) - 1] = checksumB;
    
    if (I2C_Send_UBX_CFG(I2C1, M9N_ADDR, ubx_msg, sizeof(ubx_msg)) == GPS_ERROR) {
        return GPS_ERROR;
    }

    return GPS_OK;
}

GPS_Status Get_Position(GPS_Data* data) {
    // TODO: Get the Position and Speed Data
}

GPS_Status I2C_Send_UBX_CFG(I2C_TypeDef* I2C, uint8_t dev, uint8_t* msg, size_t msg_len) {
    uint8_t UBXAck[10];
    UBX_Parser parser = {
        .buffer = {0},
        .index = 0
    };

    // Send UBX message
    I2C_Write(I2C, dev, msg, msg_len);

    volatile uint16_t len = 0;
    
    while (len == 0) {
        getAvailableBytes(I2C, dev, &len);
    }

    // Check for UBX-ACK-ACK
    // I2C_Read(I2C, dev, &parser.buffer, M9N_DATA_REG, len);

    for (size_t i = 0; i < len; i++) {
        parser.buffer[i] = I2C_Read_Reg(I2C, dev, M9N_DATA_REG);
    }

    // while (parser.index < (sizeof(parser.buffer) - 1)) {
    //     if (parser.buffer[parser.index] == UBX_PREABLE1 && 
    //         parser.buffer[parser.index + 1] == UBX_PREABLE2) {
    //         break;
    //     }
    //     parser.index++;
    // }

    if (parser.buffer[UBX_ACK_CLASS] == msg[UBX_CLASS_Pos] && 
        parser.buffer[UBX_ACK_ID] == msg[UBX_ID_Pos]) {
        return GPS_OK;
    } else {
        return GPS_ERROR;
    }
}
