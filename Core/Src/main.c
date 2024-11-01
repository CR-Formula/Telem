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

  // TODO: Split Init into Peripherals and Devices
  // TODO: Create Init checks that reruns if failed
  // Initialize Hardware
  Sysclk_168();
  LED_Init();
  I2C1_Init();
  CAN1_Init();
  CAN_Filters_Init();
  CAN_Start();
  SPI2_Init();

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
  while(1) {
    osDelay(1000);
    Toggle_Pin(GPIOC, STATUS_LED_PIN);
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

  while(1) {
    CAN_Transmit(CAN1, &tFrame);
    Receive = CAN_Receive(CAN1, &rFrame);
    if (Receive == CAN_OK) {
      telemetry.RPM = rFrame.data[0] << 8 | rFrame.data[1];
      // TODO: Implement CAN Task
    }
    osDelay(1000);
  }
}

void GPS_Task() {
  GPS_Status status;
  volatile GPS_Data data;
  osDelay(500); // Delay for GPS Module to Boot
  status = GPS_Init();

  while (status != GPS_OK) {
    status = GPS_Init();
    osDelay(1000); // Wait for GPS to recover
  }

  while(1) {
    if (Get_Position(&data) == GPS_OK) {
      telemetry.latGPS = data.latitude;
      telemetry.longGPS = data.longitude;
      telemetry.Speed = data.speed;
    }
    else {
      osDelay(100); // Wait for GPS to recover
    }
    osDelay(40); // 25Hz rate = 40ms period
  }
}

void Lora_Task() {
  LoRa_Status status;
  Set_Pin(GPIOB, 12); // Reset LoRa
  Clear_Pin(GPIOB, 12);
  status = Lora_Init();
  while (status != LORA_OK) {
    status = Lora_Init();
    osDelay(1000); // Wait for LoRa to recover
  }

  while(1) {
    // TODO: Implement LoRa Task
    // osDelay(1000);
  }
}

void Error_Handler() {
  Set_Pin(GPIOC, STATUS_LED_PIN);
  while(1);
}