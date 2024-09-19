/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/
// OS Specific Includes
#include "FreeRTOS.h" /* Must come first. */
#include "task.h" /* RTOS task related API prototypes. */
#include "queue.h" /* RTOS queue related API prototypes. */
#include "timers.h" /* Software timer related API prototypes. */
#include "semphr.h" /* Semaphore related API prototypes. */
// Hardware Specific Includes
#include "stm32f4xx_hal.h"
#include "stm32f415xx.h"

#include <stdint.h>

#include "gpio.h"
#include "can.h"
#include "i2c.h"

/* Macros  ------------------------------------------------------------------*/
#define STATUS_LED_PIN         13
#define GPS_ADDR              0x42

/* Data Structures  ---------------------------------------------------------*/
typedef struct {
  uint16_t RPM;           // RPM
  uint16_t TPS;           // TPS
  uint16_t FOT;           // Fuel Open Time
  uint16_t IA;            // Ignition Angle
  uint16_t Lam;           // Lambda
  uint16_t AirT;          // Air Temp
  uint16_t CoolT;         // Coolant Temp
  uint16_t Speed;         // Vehicle Speed
  uint16_t OilP;          // Oil Pressure
  uint16_t FRTemp;        // Front Right Brake Temp
  uint16_t RRTemp;        // Rear Right Brake Temp
  uint16_t FRPot;         // Front Right Suspension Damper
  uint16_t RRPot;         // Rear Right Suspension Damper
  uint16_t latGPS;        // Latitude GPS
  uint16_t longGPS;       // Longitude GPS
  uint16_t AccX;          // Accelerometer X Axis
  uint16_t AccZ;          // Accelerometer Z Axis
  uint16_t AccY;          // Accelerometer Y Axis
} Telemetry;

/* Functions prototypes -----------------------------------------------------*/
void Error_Handler(void);

/**
 * @brief Thread for blinking the status led
 * 
 * @param argument 
 */
void Status_LED();

/**
 * @brief Thread for handling CAN communication
 * @note TODO: Implement CAN Task
 * 
 * @param argument 
 */
void CAN_Task();

/**
 * @brief Thread for handling GPS communication
 * @note TODO: Implement GPS Task
 * 
 * @param argument 
 */
void GPS_Task();

/**
 * @brief Main Function to start FreeRTOS and initialize peripherals
 * 
 */
void main();

#endif /* __MAIN_H */
