/************************************************
* @file    gps.h 
* @author  APBashara
* @date    9/2024
* 
* @brief   Neo-M9N GPS Driver Header
***********************************************/

#include <stddef.h>
#include "i2c.h"

/* Macros -------------------------------------------------------------------*/
#define M9N_ADDR                        (0x42)
#define M9N_MSB_REG                     (0xFD)
#define M9N_LSB_REG                     (0xFE)
#define M9N_DATA_REG                    (0xFF)

#define UBX_PREABLE1                    (0xB5)
#define UBX_PREABLE2                    (0x62)

#define UBX_ACK_CLASS                   (0x05)
#define UBX_ACK_ID                      (0x01)

#define UBX_CLASS_Pos                   (0x02)
#define UBX_ID_Pos                      (0x03)
#define UBX_LEN_Pos                     (0x04)
#define UBX_PAYLOAD_Pos                 (0x06)

#define UBX_PVT_CLASS                   (0x01)
#define UBX_PVT_ID                      (0x07)
#define UBX_PVT_LEN                     (0x92)
#define UBX_PVT_FIX_Pos                 (0x14 + UBX_PAYLOAD_Pos)
#define UBX_PVT_LON_Pos                 (0x18 + UBX_PAYLOAD_Pos)
#define UBX_PVT_LAT_Pos                 (0x1C + UBX_PAYLOAD_Pos)
#define UBX_PVT_SPD_Pos                 (0x3C + UBX_PAYLOAD_Pos)

#define RETRY_COUNT                     (25)

/* Structs and Enums --------------------------------------------------------*/
typedef struct {
    uint8_t preable1;
    uint8_t preable2;
    uint8_t class;
    uint8_t id;
    uint16_t len;
    uint8_t* payload;
    uint8_t CK_A;
    uint8_t CK_B;
} UBX_Message;

typedef struct {
    uint8_t buffer[276]; // Largest UBX Packet Size
    size_t index;
} UBX_Parser;

typedef enum {
    GPS_OK,
    GPS_ERROR,
    GPS_NO_FIX,
} GPS_Status;

typedef struct {
    int32_t latitude;
    int32_t longitude;
    int32_t speed;
} GPS_Data;

/* Function Prototypes ------------------------------------------------------*/

/**
 * @brief Initialize GPS Module
 * @note Configures over I2C1
 * @note Uses UBX Messages
 * @return GPS_Status
 */
GPS_Status GPS_Init();

/**
 * @brief Checks for GPS lock and gets the current position and speed
 * 
 * @param data [GPS_Data*] Pointer to GPS Data Struct
 * @return GPS_Status
 */
GPS_Status Get_Position(GPS_Data* data);

/**
 * @brief Send and read a UBX message over I2C
 * @todo Need to return/pass a response buffer so application has access to message
 * 
 * @param I2C [I2C_TypeDef*] Peripheral to use
 * @param dev [uint8_t] Address of device [7-bit]
 * @param msg [uint8_t*] UBX message to send
 * @param msg_len [size_t] Length of UBX message

 * @return GPS_Status
 */
GPS_Status I2C_Send_UBX_CFG(I2C_TypeDef* I2C, uint8_t dev, uint8_t* msg, size_t msg_len);