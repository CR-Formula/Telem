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
  .stack_size = 128 * 4
};

osThreadId_t CANTask;
const osThreadAttr_t CANTask_attr = {
  .name = "CAN_Task",
  .priority = osPriorityNormal,
  .stack_size = 128 * 4
};

osThreadId_t GPSTask;
const osThreadAttr_t GPSTask_attr = {
  .name = "GPS_Task",
  .priority = osPriorityNormal,
  .stack_size = 128 * 4
};

void main() {
  osKernelInitialize(); // Initialize FreeRTOS

  // Initialize Peripherals
  Sysclk_168();
  LED_Init();
  I2C1_Init();
  CAN1_Init();
  CAN_Filters_Init();
  CAN_Start();

  // Create FreeRTOS Threads
  StatusLED = osThreadNew(Status_LED, NULL, &StatusLED_attr);
  CANTask = osThreadNew(CAN_Task, NULL, &CANTask_attr);
  GPSTask = osThreadNew(GPS_Task, NULL, &GPSTask_attr);

  osKernelStart(); // Start FreeRTOS
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
    .id = 0x048,
    .data = {0, 1, 2, 3, 4, 5, 6, 7},
    .dlc = 8,
    .rtr = CAN_RTR_Data
  };
  volatile CAN_Frame tFrame;
  volatile CAN_Status Receive;

  while(1) {
    CAN_Transmit(CAN1, &tFrame);
    tFrame.data[0]++;
    tFrame.data[1]+=2;
    tFrame.data[2]+=3;
    tFrame.data[3]+=4;
    tFrame.data[4]+=5;
    tFrame.data[5]+=6;
    tFrame.data[6]+=7;
    tFrame.data[7]+=8;
    osDelay(100);
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