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

  // TODO: May need more RTOS configs
  // https://www.freertos.org/Documentation/02-Kernel/03-Supported-devices/04-Demos/Device-independent-demo/Hardware-independent-RTOS-example

  // Create FreeRTOS Tasks
  Task_Status &= xTaskCreate(Status_LED, "Status_Task", 128, NULL, 1, NULL);
  Task_Status &= xTaskCreate(CAN_Task, "CAN_Task", 128, NULL, 1, NULL);
  Task_Status &= xTaskCreate(GPS_Task, "GPS_Task", 128, NULL, 1, NULL);

  if (Task_Status != pdPASS) {
    Error_Handler();
  }

  vTaskStartScheduler(); // Start FreeRTOS Scheduler

  while(1);
}

void Status_LED() {
  while(1) {
    osDelay(100);
    Toggle_Pin(GPIOC, STATUS_LED_PIN);
  }
}

void CAN_Task() {
  volatile CAN_Frame rFrame = {
    .id = 0x123,
    .data = {8, 6, 5, 3, 2, 4, 1, 5},
    .dlc = 8,
    .rtr = CAN_RTR_Data
  };
  volatile CAN_Frame tFrame;
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
  volatile uint8_t data;
  volatile uint16_t lat, lon;

  // TODO: Implement pg 27 of GPS Integration Manual
  while(1) {
    osDelay(1000);
  }
}

void Error_Handler() {
  Set_Pin(GPIOC, STATUS_LED_PIN);
  while(1);
}