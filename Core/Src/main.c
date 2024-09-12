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
uint16_t adc_value[16];

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

osThreadId_t DisplayTask;
const osThreadAttr_t DisplayTask_attr = {
  .name = "Display_Task",
  .priority = osPriorityNormal,
  .stack_size = 128
};

osThreadId_t GPSTask;
const osThreadAttr_t GPSTask_attr = {
  .name = "GPS_Task",
  .priority = osPriorityNormal,
  .stack_size = 128
};

osThreadId_t LoraTask;
const osThreadAttr_t LoraTask_attr = {
  .name = "Lora_Task",
  .priority = osPriorityNormal,
  .stack_size = 128
};

osThreadId_t ADCTask;
const osThreadAttr_t ADC_Task_attr = {
  .name = "ADC_Task",
  .priority = osPriorityNormal,
  .stack_size = 128
};

void main() {
  osKernelInitialize(); // Initialize FreeRTOS

  // Initialize Peripherals
  Sysclk_168();
  DMA_ADC1_Init(&adc_value);
  LED_Init();
  USART3_Init();
  I2C1_Init();
  SPI2_Init();
  CAN1_Init();
  CAN_Filters_Init();
  CAN_Start();

  // Create FreeRTOS Threads
  StatusLED = osThreadNew(Status_LED, NULL, &StatusLED_attr);
  CANTask = osThreadNew(CAN_Task, NULL, &CANTask_attr);
  DisplayTask = osThreadNew(Display_Update, NULL, &DisplayTask_attr);
  GPSTask = osThreadNew(GPS_Update, NULL, &GPSTask_attr);
  LoraTask = osThreadNew(Lora_Send, NULL, &LoraTask_attr);
  ADCTask = osThreadNew(ADC_Update, NULL, &ADC_Task_attr);

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

void Display_Update(void *argument) {
  uint8_t message[64];

  while(1) {
    sprintf(message, "RPM: %d\n", telemetry.RPM);
    send_String(USART3, message);
    osDelay(1000);
  }
}

void GPS_Update(void *argument) {
  uint8_t message[64];

  while(1) {
    // TODO: Implement GPS Task
  }
}

void Lora_Send(void *argument) {

  while(1) {
    // TODO: Implement Lora Task
  }
}

void ADC_Update(void *argument) {

  while(1) {
    // TODO: Set update rates
    telemetry.TPS = adc_value[0];
    telemetry.FOT = adc_value[1];
    telemetry.IA = adc_value[2];
    osDelay(1000);
  }
}