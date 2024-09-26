/************************************************
* @file    gps.h 
* @author  APBashara
* @date    9/2024
* 
* @brief   Neo-M9N GPS Driver Header
***********************************************/


#include <stddef.h>
#include "i2c.h"

#define GPS_ADDR                        0x42
#define UBX_PREABLE1                    0xB5
#define UBX_PREABLE2                    0x62
#define UBX_CLASS_Pos                   0x02
#define UBX_ID_Pos                      0x03
#define UBX_LEN_Pos                     0x04
#define UBX_PAYLOAD_START               0x06
#define UBX_ACK_CLASS                   0x06
#define UBX_ACK_ID                      0x07


typedef struct {
    uint8_t preable1;
    uint8_t preable2;
    uint8_t class;
    uint8_t id;
    uint8_t len;
    uint8_t* payload;
    uint8_t CK_A;
    uint8_t CK_B;
} UBX_Message;

typedef enum {
    GPS_OK = 0,
    GPS_ERROR = 1
} GPS_Status;

typedef struct {
    uint32_t latitude;
    uint32_t longitude;
    uint32_t speed;
} GPS_Data;


/**
 * @brief Initialize GPS Module
 * @note Configures over I2C1
 * @note Uses UBX Messages
 * @return GPS_Status
 */
GPS_Status GPS_Init();

/**
 * @brief Get GPS Position Data
 * 
 * @param data [GPS_Data*] Pointer to GPS Data Struct
 * @return GPS_Status
 */
GPS_Status Get_Position(GPS_Data* data);

/**
 * @brief Send and read a UBX message over I2C
 * 
 * @param I2C [I2C_TypeDef*] Peripheral to use
 * @param dev [uint8_t] Address of device [7-bit]
 * @param msg [uint8_t*] UBX message to send
 * @param msg_len [size_t] Length of UBX message

 * @return GPS_Status
 */
GPS_Status I2C_Send_UBX(I2C_TypeDef* I2C, uint8_t dev, uint8_t* msg, size_t msg_len);