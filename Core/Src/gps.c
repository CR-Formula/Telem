/************************************************
* @file    gps.c 
* @author  APBashara
* @date    9/2024
* 
* @brief   Neo-M9N GPS Driver Implementation
***********************************************/

#include "gps.h"

uint8_t buffer[512]; // Largest UBX Packet Size

/* Static Functions ---------------------------------------------------------*/
static GPS_Status checkACK(uint8_t* response, size_t msg_len, uint8_t class, uint8_t id) {
    if (response[UBX_ACK_CLASS] == class && 
        response[UBX_ACK_ID] == id) {
        return GPS_OK;
    } else {
        return GPS_ERROR;
    }
}

static GPS_Status calcChecksum(const uint8_t *data, size_t length, uint8_t *ckA, uint8_t *ckB) {
    uint8_t sumA = 0;
    uint8_t sumB = 0;

    for (size_t i = 2; i < length; i++) {
        sumA = sumA + data[i];
        sumB = sumB + sumA;
    }

    *ckA = sumA;
    *ckB = sumB;
    return GPS_OK;
}

static uint16_t getAvailableBytes(I2C_TypeDef* I2C, uint8_t dev) {
    uint16_t len = 0;
    uint8_t data[2];
    uint8_t count = 0;

    // Read length of data
    I2C_Read(I2C, dev, M9N_MSB_REG, data, 2); // Read 0xFD and 0xFE registers
    len = (uint16_t)(data[0] << 8 | data[1]);

    if (len > sizeof(buffer)) {
        len = sizeof(buffer);
    }

    return len;
}


/* Function Implementation --------------------------------------------------*/

GPS_Status GPS_Init() {
    GPS_Status ret_val = GPS_ERROR;
    uint8_t payload_size = 0;
    size_t msg_size = 0;
    uint8_t ubx_msg[] = {
        0xB5, 0x62,     // Sync Chars
        0x06, 0x8A,     // Class (CFG), ID (VALSET)
        0x24, 0x00,     // Length of payload
        0x00,           // Version (0x00)
        0x01,           // Layer (0x01 for RAM, or 0x04 for Flash)
        0x00, 0x00,     // Reserved for Transactions

        // Measurement & navigation rate (25 Hz -> 40 ms)
        0x01, 0x00, 0x21, 0x30,         // CFG-RATE-MEAS (0x30210001)
        0x28, 0x00,                     // 40ms
        0x02, 0x00, 0x21, 0x30,         // CFG-RATE-NAV (0x30210002)
        0x01, 0x00,                     // NavRate=1 (every meas)
        0x03, 0x00, 0x21, 0x20,         // CFG-RATE-TIMEREF (0x20210003)
        0x01,                           // TimeRef=1 (GPS)

        // Active-antenna voltage control
        0x2E, 0x00, 0xA3, 0x10,         // CFG-HW-ANT_CFG_VOLTCTRL (0x10A3002E)
        0x01,                           // Enable voltage control

        // Enable UBX output on I2C port
        0x01, 0x00, 0x72, 0x10,         // CFG-I2COUTPROT-UBX (0x10720001)
        0x01,                           // Enable UBX output

        // NAV-PVT output rate on I2C (25 Hz)
        0x06, 0x00, 0x91, 0x20,         // CFG-MSGOUT-UBX_NAV_PVT_I2C (0x20910006)
        0x01,                           // Enable PVT on I2C

        0xE6, 0x95                      // CK_A, CK_B (Fletcher) 
    };

    msg_size = sizeof(ubx_msg);
    // calcChecksum(&ubx_msg, ubx_msg[UBX_LEN_Pos], 
    //     &ubx_msg[msg_size - 2], &ubx_msg[msg_size - 1]);

    if (I2C_Send_UBX_CFG(I2C1, M9N_ADDR, ubx_msg, msg_size) == GPS_ERROR) {
        ret_val = GPS_ERROR;
    }
    else {
        ret_val = GPS_OK;
    }

    return ret_val;
}

GPS_Status Get_Position(GPS_Data* data) {
    volatile uint16_t len = 0;
    uint8_t count = 0;
    uint8_t poll_nav_pvt[] = {
        UBX_PREABLE1, UBX_PREABLE2, // Sync Chars
        0x01, 0x07, // Class (NAV), ID (PVT)
        0x00, 0x00, // Length of payload (0 bytes)
        0x08, 0x19  // Checksum
    };

    // Write PVT Poll Request
    I2C_Write(I2C1, M9N_ADDR, poll_nav_pvt, sizeof(poll_nav_pvt));

    while (len == 0 && count < RETRY_COUNT) {
        len = getAvailableBytes(I2C1, M9N_ADDR);
        count++;
    }
    if (len == 0 || count == RETRY_COUNT) {
        return GPS_ERROR;
    }

    I2C_Read(I2C1, M9N_ADDR, M9N_DATA_REG, buffer, len);

    // Data is sent in signed little-endian 32-bit integer, two's complement
    data->latitude = buffer[UBX_PVT_LAT_Pos + 3] << 24 |
                     buffer[UBX_PVT_LAT_Pos + 2] << 16 |
                     buffer[UBX_PVT_LAT_Pos + 1] << 8 |
                     buffer[UBX_PVT_LAT_Pos];
    data->longitude = buffer[UBX_PVT_LON_Pos + 3] << 24 |
                      buffer[UBX_PVT_LON_Pos + 2] << 16 |
                      buffer[UBX_PVT_LON_Pos + 1] << 8 |
                      buffer[UBX_PVT_LON_Pos];
    data->speed = buffer[UBX_PVT_SPD_Pos + 3] << 24 |
                  buffer[UBX_PVT_SPD_Pos + 2] << 16 |
                  buffer[UBX_PVT_SPD_Pos + 1] << 8 |
                  buffer[UBX_PVT_SPD_Pos];
    
    return GPS_OK;
}

GPS_Status I2C_Send_UBX_CFG(I2C_TypeDef* I2C, uint8_t dev, uint8_t* msg, size_t msg_len) {
    volatile uint16_t len = 0;

    // Send UBX message
    I2C_Write(I2C, dev, msg, msg_len);
    
    while (len == 0) {
        len = getAvailableBytes(I2C, dev);
    }

    // Check for Max Length
    if (len > sizeof(buffer)) {
        len = sizeof(buffer);
    }

    // Check for UBX-ACK-ACK
    I2C_Read(I2C, dev, M9N_DATA_REG, buffer, len);

    // Check for ACK-ACK response
    if (buffer[UBX_ACK_CLASS] == UBX_ACK_CLASS && 
        buffer[UBX_ACK_ID] == UBX_ACK_ID) {
        return GPS_OK;
    } else {
        return GPS_ERROR;
    }
}
