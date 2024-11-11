/************************************************
* @file    main.c 
* @author  APBashara
* @date    9/2024
* 
* @brief   Main Code to run Tasks and Setup Peripherals
***********************************************/

#include "main.h"

/* Global Variables ---------------------------------------------------------*/
Telemetry telemetry;
uint16_t ADC_Buffer[16];

/* Function Calls -----------------------------------------------------------*/
void main() {
  uint8_t Task_Status = 1;

  // TODO: Split Init into Peripherals and Devices
  // TODO: Create Init checks that reruns if failed
  // Initialize Hardware
  Sysclk_168();
  LED_Init();
  I2C1_Init();
  CAN1_Init();
  CAN_Filters_Init();
  CAN_Start();
  DMA_ADC1_Init(&ADC_Buffer);

  // Create FreeRTOS Tasks
  Task_Status &= xTaskCreate(Status_LED, "Status_Task", 128, NULL, 2, NULL);
  Task_Status &= xTaskCreate(CAN_Task, "CAN_Task", 256, NULL, 2, NULL);
  Task_Status &= xTaskCreate(GPS_Task, "GPS_Task", 512, NULL, 1, NULL);
  Task_Status &= xTaskCreate(ADC_Task, "ADC_Task", 128, NULL, 1, NULL);

  if (Task_Status != pdPASS) {
    Error_Handler();
  }

  vTaskStartScheduler(); // Start FreeRTOS Scheduler

  while(1);
}

void Status_LED() {
  const TickType_t StatusFrequency = 1000;
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while(1) {
    Toggle_Pin(GPIOC, STATUS_LED_PIN);
    vTaskDelayUntil(&xLastWakeTime, StatusFrequency);
  }
}

void CAN_Task() {
  CAN_Frame tFrame = {
    .id = 0x123,
    .data = {8, 6, 5, 3, 2, 4, 1, 5},
    .dlc = 8,
    .rtr = CAN_RTR_Data
  };
  volatile CAN_Frame rFrame;
  volatile CAN_Status Receive;
  const TickType_t CANFrequency = 1000;

  TickType_t xLastWakeTime = xTaskGetTickCount();

  while(1) {
    CAN_Transmit(CAN1, &tFrame);
    Receive = CAN_Receive(CAN1, &rFrame);
    if (Receive == CAN_OK) {
      telemetry.RPM = rFrame.data[0] << 8 | rFrame.data[1];
      // TODO: Implement CAN Task
    }
    vTaskDelayUntil(&xLastWakeTime, CANFrequency);
  }
}

void GPS_Task() {
  GPS_Status status;
  volatile GPS_Data data;
  const TickType_t GPSFrequency = 40; // 25Hz
  vTaskDelay(500); // Delay for GPS Module to Boot
  status = GPS_Init();

  while (status != GPS_OK) {
    status = GPS_Init();
    vTaskDelay(1000); // Wait for GPS to recover
  }

  TickType_t xLastWakeTime = xTaskGetTickCount();

  while(1) {
    if (Get_Position(&data) == GPS_OK) {
      telemetry.latGPS = data.latitude;
      telemetry.longGPS = data.longitude;
      telemetry.Speed = data.speed;
    }
    else {
      vTaskDelay(100); // Wait for GPS to recover
    }
    vTaskDelayUntil(&xLastWakeTime, GPSFrequency); // 25Hz rate = 40ms period
  }
}

void ADC_Task() {
  while(1) {
    telemetry.FRPot = ADC_Buffer[0];
    telemetry.RRPot = ADC_Buffer[1];
    telemetry.FRTemp = ADC_Buffer[2];
    telemetry.RRTemp = ADC_Buffer[3];
    osDelay(10); // 100Hz rate = 10ms period
  }
}

void Error_Handler() {
  Set_Pin(GPIOC, STATUS_LED_PIN);
  while(1);
}