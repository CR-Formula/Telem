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

uint16_t getAvailableBytes(I2C_TypeDef* I2C, uint8_t dev) {
    uint16_t len = 0;
    uint8_t data[2];
    uint8_t count = 0;

    // Read length of data
    data[0] = I2C_Read_Reg(I2C, dev, M9N_MSB_REG);
    data[1] = I2C_Read_Reg(I2C, dev, M9N_LSB_REG);
    len = data[0] << 8 | data[1];

    return len;
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
        0x17, 0x87  // Checksum
    };
    
    if (I2C_Send_UBX_CFG(I2C1, M9N_ADDR, ubx_msg, sizeof(ubx_msg)) == GPS_ERROR) {
        return GPS_ERROR;
    }

    return GPS_OK;
}

GPS_Status Get_Position(GPS_Data* data) {
    volatile uint16_t len = 0;
    // uint8_t count = 0;
    uint8_t poll_nav_pvt[] = {
        UBX_PREABLE1, UBX_PREABLE2, // Sync Chars
        0x01, 0x07, // Class (NAV), ID (PVT)
        0x00, 0x00, // Length of payload (0 bytes)
        0x08, 0x19  // Checksum
    };
    UBX_Parser parser = {
        .buffer = {0},
        .index = 0
    };

    // Write PVT Poll Request
    I2C_Write(I2C1, M9N_ADDR, poll_nav_pvt, sizeof(poll_nav_pvt));

    while (len == 0) {
        len = getAvailableBytes(I2C1, M9N_ADDR);
    }

    I2C_Read(I2C1, M9N_ADDR, M9N_DATA_REG, parser.buffer, len);

    // Data is sent in signed little-endian 32-bit integer, two's complement
    data->latitude = parser.buffer[UBX_PVT_LAT_Pos + 3] << 24 |
                     parser.buffer[UBX_PVT_LAT_Pos + 2] << 16 |
                     parser.buffer[UBX_PVT_LAT_Pos + 1] << 8 |
                     parser.buffer[UBX_PVT_LAT_Pos];
    data->longitude = parser.buffer[UBX_PVT_LON_Pos + 3] << 24 |
                      parser.buffer[UBX_PVT_LON_Pos + 2] << 16 |
                      parser.buffer[UBX_PVT_LON_Pos + 1] << 8 |
                      parser.buffer[UBX_PVT_LON_Pos];
    data->speed = parser.buffer[UBX_PVT_SPD_Pos + 3] << 24 |
                  parser.buffer[UBX_PVT_SPD_Pos + 2] << 16 |
                  parser.buffer[UBX_PVT_SPD_Pos + 1] << 8 |
                  parser.buffer[UBX_PVT_SPD_Pos];
    
    return GPS_OK;
}

GPS_Status I2C_Send_UBX_CFG(I2C_TypeDef* I2C, uint8_t dev, uint8_t* msg, size_t msg_len) {
    volatile uint16_t len = 0;
    UBX_Parser parser = {
        .buffer = {0},
        .index = 0
    };

    // Send UBX message
    I2C_Write(I2C, dev, msg, msg_len);
    
    while (len == 0) {
        len = getAvailableBytes(I2C, dev);
    }

    // Check for Max Length
    if (len > sizeof(parser.buffer)) {
        len = sizeof(parser.buffer);
    }

    // Check for UBX-ACK-ACK
    I2C_Read(I2C, dev, M9N_DATA_REG, parser.buffer, len);

    if (parser.buffer[UBX_ACK_CLASS] == msg[UBX_CLASS_Pos] && 
        parser.buffer[UBX_ACK_ID] == msg[UBX_ID_Pos]) {
        return GPS_OK;
    } else {
        return GPS_ERROR;
    }
}
