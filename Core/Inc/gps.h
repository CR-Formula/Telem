/************************************************
* @file    gps.h 
* @author  APBashara
* @date    9/2024
* 
* @brief   Neo-M9N GPS Driver Header
***********************************************/


#include <stddef.h>
#include "i2c.h"

typedef struct {
    uint32_t latitude;
    uint32_t longitude;
    uint32_t speed;
} GPS_Data;


static void calculateChecksum(uint8_t* CK, uint8_t* data, size_t len);

/**
 * @brief Initialize GPS Module
 * @note Configures over I2C1
 * @note Uses UBX Messages
 */
void GPS_Init();

/**
 * @brief Get GPS Position Data
 * 
 * @param data [GPS_Data*] Pointer to GPS Data Struct
 */
void Get_Position(GPS_Data* data);