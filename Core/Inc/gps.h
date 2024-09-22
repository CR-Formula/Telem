/************************************************
* @file    gps.h 
* @author  APBashara
* @date    9/2024
* 
* @brief   Neo-M9N GPS Driver Header
***********************************************/

#include "i2c.h"

static void calculateChecksum(uint8_t* CK, uint8_t* data, size_t len);

/**
 * @brief Initialize GPS Module
 * @note Configures over I2C1
 * @note Uses UBX Messages
 */
void GPS_Init();