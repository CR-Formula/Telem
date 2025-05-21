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

#ifndef GPIO_H
    #define GPIO_H
    #include "gpio.h"
#endif
#include "can.h"
#include "adc.h"
#include "timer.h"
#include "uart.h"
#include "gps.h"
#include "lora.h"

/* Macros  ------------------------------------------------------------------*/
// Constant Definitions
#define STATUS_LED_PIN              (13)
#define GPS_ADDR                    (0x42)
#define GPS_RST_PIN                 (8)
#define LORA_RST_PIN                (8)
#define SUS_POT_TRAVEL              (50)
#define THERMOCOUPLE_CONVERSION     (100)

// Priotity Definitions -- Higher number = Higher Priority
#define ADC_PRIORITY                (configMAX_PRIORITIES - 1)
#define GPS_PRIORITY                (configMAX_PRIORITIES - 3)
#define CAN_PRIORITY                (configMAX_PRIORITIES - 5)
#define LED_PRIORITY                (configMAX_PRIORITIES - 7)
#define STATS_PRIORITY              (configMAX_PRIORITIES - 8)

#define LORA_SUSPENSION_PRIORITY    (configMAX_PRIORITIES - 2)
#define LORA_GPS_PRIORITY           (configMAX_PRIORITIES - 4)
#define LORA_ENGINE_PRIORITY        (configMAX_PRIORITIES - 6)
#define LORA_BRAKES_ACCEL_PRIORITY  (configMAX_PRIORITIES - 6)
#define LORA_TEMPERATURE_PRIORITY   (configMAX_PRIORITIES - 7)

// LoRa Packet IDs
#define LORA_SUSPENSION_ID          (0x01) // 50 Hz
#define LORA_GPS_ID                 (0x02) // 25 Hz
#define LORA_ENGINE_ID              (0x03) // 20 Hz
#define LORA_BRAKES_ACCEL_ID        (0x04) // 10 Hz
#define LORA_TEMPERATURE_ID         (0x05) // 1 Hz

// ADC Channel Assignments
#define Thermocouple_1_ADC          (0u)
#define Thermocouple_2_ADC          (1u)
#define Thermocouple_3_ADC          (10u)
#define Thermocouple_4_ADC          (11u)
#define Thermocouple_5_ADC          (12u)
#define Thermocouple_6_ADC          (13u)
#define Steering_Angle_ADC          (4u)
#define Throttle_Position_1_ADC     (5u)
#define Throttle_Position_2_ADC     (7u)
#define Brake_Position_ADC          (6u)
#define Sus_Pot_1_ADC               (8u)
#define Sus_Pot_2_ADC               (9u)
#define Sus_Pot_3_ADC               (14u)
#define Sus_Pot_4_ADC               (15u)
// #define NA_ADC                      (2u)
// #define NA_ADC                      (3u)

/* Data Structures  ---------------------------------------------------------*/
/**
 * @brief 50 Hz packet with ID 0x01
 * @note  Contains Front and Rear Suspension Potentiometer values
 */
typedef struct {
  const uint8_t PacketID;           // ID = 0x01

  uint16_t FrontPot;                // Front Right Suspension Damper
  uint16_t RearPot;                 // Rear Right Suspension Damper

} LoRa_Suspension_Packet;

/**
 * @brief 25 Hz packet with ID 0x02
 * @note  Contains GPS Latitude, Longitude, and Speed
 */
typedef struct {
  const uint8_t PacketID;           // ID = 0x02

  int32_t latGPS;                   // Latitude GPS
  int32_t longGPS;                  // Longitude GPS
  int32_t Speed;                    // Vehicle GPS Speed

} LoRa_GPS_Packet;

/**
 * @brief 20 Hz packet with ID 0x03
 * @note  Contains Engine RPM, Throttle Position, Steering Angle, and Brake Pressure
 */
typedef struct {
  const uint8_t PacketID;           // ID = 0x03

  uint16_t BrakePressure;           // Brake Pressure
  uint16_t ThrottleADC;             // Analog Throttle Position
  uint16_t Steering;                // Steering Angle
  uint16_t RPM;                     // Engine RPM
  uint16_t ThrottlePosSensor;       // Throttle Position from ECU
  uint16_t Lambda;                  // Lambda

} LoRa_Engine_Data_Packet;

/**
 * @brief 10 Hz packet with ID 0x04
 * @note  Contains Oil Pressure, Front and Rear Brake Temp, and Accelerometer values
 */
typedef struct {
  const uint8_t PacketID;           // ID = 0x04

  uint16_t OilPressure;             // Oil Pressure
  uint16_t FrontBrakeTemp;          // Front Right Brake Temp (F)
  uint16_t RearBrakeTemp;           // Rear Right Brake Temp (F)
  uint16_t AccelX;                  // Accelerometer X Axis
  uint16_t AccelZ;                  // Accelerometer Z Axis
  uint16_t AccelY;                  // Accelerometer Y Axis

} LoRa_Brakes_Accel_Packet;

/**
 * @brief 1 Hz packet with ID 0x05
 * @note  Contains Air and Coolant Temp
 */
typedef struct {
  const uint8_t PacketID;           // ID = 0x05

  uint16_t AirTemp;                 // Air Temp (F)
  uint16_t CoolTemp;                // Coolant Temp (F)

} LoRa_Temperature_Packet;

/**
 * @brief Telemetry Struct to hold all Telemetry Data
 * @note  Anything not in a LoRa packet should be in this struct
 */
typedef struct {
  // LoRa Packets
  LoRa_Suspension_Packet Suspension_Packet;           // 50 Hz
  LoRa_GPS_Packet GPS_Packet;                         // 25 Hz
  LoRa_Engine_Data_Packet Engine_Data_Packet;         // 20 Hz
  LoRa_Brakes_Accel_Packet Brakes_Accel_Packet;       // 10 Hz
  LoRa_Temperature_Packet Temperature_Packet;         // 1 Hz

} Telemetry;

/* Functions prototypes -----------------------------------------------------*/

/**
 * @brief Handles Systems Errors
 * @note Holds Status LED on until reset
 * @todo Create Blink status codes for errors
 */
void Error_Handler(void);

/**
 * @brief Thread for blinking the status led
 */
void Status_LED();

/**
 * @brief Thread for handling CAN communication
 * @note Handles CAN RX with interrupts and FreeRTOS Queues
 */
void CAN_Task();

/**
 * @brief Thread for handling GPS communication
 * @note TODO: Implement GPS Task
 */
void GPS_Task();

/**
 * @brief Thread for send the Telemetry Struct over LoRa
 */
void Lora_Task();
 
 /**
 * @brief Thread for handling ADC communication
 * @note pulls values from DMA buffer and calculates Sensor values
 */
void ADC_Task();

/**
 * @brief Thread for handling Thermocouple decode
 * @note Pulls values from the ADC DMA buffer and calculates the temperature
 */
void Thermocouple_Task();

/**
 * @brief Send Suspension Data over LoRa
 * @note Packet ID 0x01 @ 50 Hz
 */
void LoRa_Suspension_Task();

/**
 * @brief Send GPS Data over LoRa
 * @note Packet ID 0x02 @ 25 Hz
 */
void LoRa_GPS_Task();

/**
 * @brief Send Engine Data over LoRa
 * @note Packet ID 0x03 @ 20 Hz
 */
void LoRa_Engine_Data_Task();

/**
 * @brief Send Brake and Acceleration Data over LoRa
 * @note Packet ID 0x04 @ 10 Hz
 */
void LoRa_Brakes_Accel_Task();

/**
 * @brief Send Temperature Data over LoRa
 * @note Packet ID 0x05 @ 1 Hz
 */
void LoRa_Temperature_Task();

/**
 * @brief Thread for collecting system statistics
 * @note Build with make STATS=1 to enable
 */
void Collect_Stats();

/**
 * @brief Main Function to start FreeRTOS and initialize peripherals
 * 
 */
void main();

#endif /* __MAIN_H */
