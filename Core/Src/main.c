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

/* Thread Attributes --------------------------------------------------------*/
osThreadId_t StatusLED;
const osThreadAttr_t StatusLED_attr = {
  .name = "Status_Task",
  .priority = osPriorityBelowNormal,
  .stack_size = 128
};

osThreadId_t CANTask;
const osThreadAttr_t CANTask_attr = {
  .name = "CAN_Task",
  .priority = osPriorityNormal,
  .stack_size = 128
};

void main() {
  osKernelInitialize(); // Initialize FreeRTOS

  // Initialize Peripherals
  Sysclk_168();
  LED_Init();
  CAN1_Init();
  CAN_Filters_Init();
  CAN_Start();

  // Create FreeRTOS Threads
  StatusLED = osThreadNew(Status_LED, NULL, &StatusLED_attr);
  CANTask = osThreadNew(CAN_Task, NULL, &CANTask_attr);

  osKernelStart(); // Start FreeRTOS
  while(1);
}

void Status_LED(void *argument) {
  while(1) {
    osDelay(100);
    Toggle_Pin(GPIOC, 13);
  }
}

void CAN_Task(void *argument) {
  volatile CAN_Frame rFrame;
  volatile CAN_Status Receive;

  while(1) {
    Receive = CAN_Receive(CAN1, &rFrame);
    if (Receive == CAN_OK) {
      telemetry.RPM = rFrame.data[0] << 8 | rFrame.data[1];
      // TODO: Implement CAN Task
    }
    osDelay(1000);
  }
}