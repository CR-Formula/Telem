/************************************************
* @file    i2c.h 
* @author  APBashara
* @date    6/2024
* 
* @brief   Prototype Functions for I2C Driver
***********************************************/

#include <stddef.h>

#include "stm32f415xx.h"

typedef enum {
    I2C_OK = 69,
    I2C_ERROR = 420
} I2C_Status;

/**
 * @brief Initialize I2C1
 * @note FM, 400kHz, 7-bit Addressing
 */
I2C_Status I2C1_Init();

/**
 * @brief Send a data byte over I2C
 * 
 * @param I2C [I2C_TypeDef*] Peripheral to use
 * @param addr [uint8_t] Address of device [7-bit]
 * @param data [uint8_t] Data to send [8-bit]
 */
void I2C_Write(I2C_TypeDef* I2C, uint8_t addr, uint8_t* data, size_t len);

/**
 * @brief Read a byte of data over I2C
 * 
 * @param I2C [I2C_TypeDef*] Peripheral to use
 * @param dev [uint8_t] Address of device [7-bit]
 * @param reg [uint8_t] Register to read
 */
uint8_t I2C_Read_Reg(I2C_TypeDef* I2C, uint8_t dev, uint8_t reg);

/**
 * @brief Read data from the I2C Line
 * 
 * @param I2C 
 * @param dev 
 * @param data 
 * @param len 
 */
I2C_Status I2C_Read(I2C_TypeDef* I2C, uint8_t dev, uint8_t* data, size_t len);