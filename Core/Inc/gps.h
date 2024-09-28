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
#define GPS_ADDR                        0x42
#define MSB_REG                         0xFD
#define LSB_REG                         0xFE
#define UBX_PREABLE1                    0xB5
#define UBX_PREABLE2                    0x62
#define UBX_CLASS_Pos                   0x02
#define UBX_ID_Pos                      0x03
#define UBX_LEN_Pos                     0x04
#define UBX_PAYLOAD_START               0x06
#define UBX_ACK_CLASS                   0x06
#define UBX_ACK_ID                      0x07
#define UBX_PVT_CLASS                   0x01
#define UBX_PVT_ID                      0x07
#define UBX_PVT_LEN                     0x92
#define UBX_PVT_LON_Pos                 0x18
#define UBX_PVT_LAT_Pos                 0x1C
#define UBX_PVT_SPD_Pos                 0x3C

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
    uint8_t buffer[92];
    size_t index;
} UBX_Parser;

typedef enum {
    GPS_OK = 0,
    GPS_ERROR = 1
} GPS_Status;

typedef struct {
    int32_t latitude;
    int32_t longitude;
    int32_t speed;
} GPS_Data;


/* Static Functions ---------------------------------------------------------*/

/**
 * @brief Static Function to check for ACK ubx message
 * 
 * @param msg [uint8_t*] Response Message to check
 * @param msg_len [size_t] Length of Response Message
 * @param class [uint8_t] Class ID to check
 * @param id [uint8_t] ID to check
 * @return GPS_Status 
 */
static GPS_Status checkACK(uint8_t* msg, size_t msg_len, uint8_t class, uint8_t id);

/**
 * @brief Static Function to calculate UBX Checksum
 * 
 * @param msg [uint8_t*] Message to calculate checksum for
 * @param msg_len [size_t] Length of Message
 * @param CK_A [uint8_t*] Pointer to CK_A
 * @param CK_B [uint8_t*] Pointer to CK_B
 * @return GPS_Status 
 */
static GPS_Status calcChecksum(uint8_t* msg, size_t msg_len, uint8_t* CK_A, uint8_t* CK_B);

/**
 * @brief Static Function to convert Big Endian to Little Endian
 * 
 * @param data [uint8_t*] Data to convert
 * @param len [size_t] Length of Data
 * @return GPS_Status 
 */
static GPS_Status littleEndian(uint8_t* data, size_t len);

/* Function Prototypes ------------------------------------------------------*/

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
 * @todo Need to return/pass a response buffer so application has access to message
 * 
 * @param I2C [I2C_TypeDef*] Peripheral to use
 * @param dev [uint8_t] Address of device [7-bit]
 * @param msg [uint8_t*] UBX message to send
 * @param msg_len [size_t] Length of UBX message

 * @return GPS_Status
 */
GPS_Status I2C_Send_UBX_CFG(I2C_TypeDef* I2C, uint8_t dev, uint8_t* msg, size_t msg_len);