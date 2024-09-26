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
    for (int i = 0; i < msg[UBX_LEN_Pos]; i++) {
        *CK_A += msg[i + UBX_PAYLOAD_START];
        *CK_B += *CK_A;
    }
    return GPS_OK;
}

static GPS_Status littleEndian(uint8_t* data, size_t len) {
    for (int i = 0; i < len; i += 2) {
        uint8_t temp = data[i];
        data[i] = data[i + 1];
        data[i + 1] = temp;
    }
    return GPS_OK;
}


/* Function Implementation --------------------------------------------------*/
GPS_Status GPS_Init() {
    uint8_t ubx_msg[] = {
        0xB5, 0x62, // Sync Chars
        0x06, 0x8A, // Class ID (CFG-MSG)
        0x06, 0x00, // Length of payload in Bytes
        // Start of Key/Value pairs
        0x30, 0x21, 0x00, 0x01, // CFG-RATE-MEAS Key
        0x28, 0x00, // CFG-RATE-MEAS Value (40ms)
        0x00, 0x00, // Checksum
    };
    uint8_t checksum[2];
    for (int i = 0; i < ubx_msg[UBX_LEN_Pos]; i++) {
        checksum[0] += ubx_msg[i + UBX_PAYLOAD_START];
        checksum[1] += checksum[0];
    }
    ubx_msg[12] = checksum[0];
    ubx_msg[13] = checksum[1];
    
    if (I2C_Send_UBX(I2C1, GPS_ADDR, ubx_msg, sizeof(ubx_msg)) == GPS_ERROR) {
        return GPS_ERROR;
    }

    return GPS_OK;
}

GPS_Status Get_Position(GPS_Data* data) {
    // TODO: Send and parse UBX message for Lat, Lng, and Speed
}

GPS_Status I2C_Send_UBX_CFG(I2C_TypeDef* I2C, uint8_t dev, uint8_t* msg, size_t msg_len) {
    uint8_t response[9];
    size_t response_len;

    // Send UBX message
    I2C_Write(I2C, dev, msg, msg_len);

    // Check for UBX-ACK-ACK
    I2C_Read(I2C, dev, &response, response_len);
    if (response[UBX_ACK_CLASS] == msg[UBX_CLASS_Pos] && 
        response[UBX_ACK_ID] == msg[UBX_ID_Pos]) {
        return GPS_OK;
    } else {
        return GPS_ERROR;
    }
}
