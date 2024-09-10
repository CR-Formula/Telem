#include "stm32f415xx.h"
#include "cmsis_os2.h"

#include <stdint.h>

#include "gpio.h"
#include "can.h"

void Status_LED(void *argument);
void CAN_Task(void *argument);

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

int main() {
  osKernelInitialize(); // Initialize FreeRTOS

  Sysclk_168();
  LED_Init();
  CAN1_Init();

  StatusLED = osThreadNew(Status_LED, NULL, &StatusLED_attr);
  CANTask = osThreadNew(CAN_Task, NULL, &CANTask_attr);

  osKernelStart(); // Start FreeRTOS
  while(1);
}

/**
 * @brief Thread for blinking the status led
 * 
 * @param argument 
 */
void Status_LED(void *argument) {
  while(1) {
    osDelay(100);
    Toggle_Pin(GPIOC, 13);
  }
}

/**
 * @brief Thread for handling CAN communication
 * 
 * @param argument 
 */
void CAN_Task(void *argument) {
  CAN_Frame frame;
  CAN_Frame received;
  volatile CAN_Status Send;
  volatile CAN_Status Receive;
  frame.id = 0x123;
  frame.dlc = 8;
  frame.rtr = CAN_RTR_Data;
  for(int i = 0; i < 8; i++) {
    frame.data[i] = i;
  }

  CAN_Filters_Init();
  CAN_Start();

  while(1) {
    Send = CAN_Transmit(CAN1, &frame);
    osDelay(1000);
    Receive = CAN_Receive(CAN1, &received);
    osDelay(1000);
  }
}