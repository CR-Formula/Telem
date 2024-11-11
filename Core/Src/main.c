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

/* Function Calls -----------------------------------------------------------*/
void main() {
  uint8_t Task_Status = 1;

  // Initialize Hardware
  Sysclk_168();
  LED_Init();
  I2C1_Init();
  CAN1_Init();
  CAN_Filters_Init();
  CAN_Start();
  SPI2_Init();
  GPIO_Init();

  // Create FreeRTOS Tasks
  Task_Status &= xTaskCreate(Status_LED, "Status_Task", 64, NULL, 2, NULL);
  Task_Status &= xTaskCreate(CAN_Task, "CAN_Task", 128, NULL, 2, NULL);
  Task_Status &= xTaskCreate(GPS_Task, "GPS_Task", 512, NULL, 1, NULL);
  Task_Status &= xTaskCreate(Lora_Task, "Lora_Task", 128, NULL, 2, NULL);

  if (Task_Status != pdPASS) {
    Error_Handler();
  }

  vTaskStartScheduler(); // Start FreeRTOS Scheduler

  while(1);
}

void Status_LED() {
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while(1) {
    Toggle_Pin(GPIOC, STATUS_LED_PIN);
    vTaskDelayUntil(&xLastWakeTime, 1000);
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

  TickType_t xLastWakeTime = xTaskGetTickCount();

  while(1) {
    CAN_Transmit(CAN1, &tFrame);
    Receive = CAN_Receive(CAN1, &rFrame);
    if (Receive == CAN_OK) {
      telemetry.RPM = rFrame.data[0] << 8 | rFrame.data[1];
      // TODO: Implement CAN Task
    }
    vTaskDelayUntil(&xLastWakeTime, 1000);
  }
}

void GPS_Task() {
  GPS_Status status;
  volatile GPS_Data data;
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
    vTaskDelayUntil(&xLastWakeTime, 40); // 25Hz rate = 40ms period
  }
}

void Lora_Task() {
  LoRa_Status status;
  uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
  Clear_Pin(LORA_IO_PORT, LORA_RST);
  osDelay(10);
  Set_Pin(LORA_IO_PORT, LORA_RST);
  osDelay(100);
  status = Lora_Init();

  while(1) {
    Lora_Transmit((uint8_t*)&data, sizeof(data));
    vTaskDelay(1000);
  }
}

void Error_Handler() {
  Set_Pin(GPIOC, STATUS_LED_PIN);
  while(1);
}