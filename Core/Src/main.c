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

  // Initialize Hardware
  Sysclk_168();
  LED_Init();
  I2C1_Init();
  CAN1_Init();
  CAN_Filters_Init();
  CAN_Start();
  SPI2_Init();
  GPIO_Init();
  DMA_ADC1_Init(&ADC_Buffer);
  USART3_Init();

  // Create FreeRTOS Tasks
  Task_Status &= xTaskCreate(Status_LED, "Status_Task", 128, NULL, LED_PRIORITY, NULL);
  Task_Status &= xTaskCreate(CAN_Task, "CAN_Task", 256, NULL, CAN_PRIORITY, NULL);
  // Task_Status &= xTaskCreate(GPS_Task, "GPS_Task", 512, NULL, GPS_PRIORITY, NULL);
  Task_Status &= xTaskCreate(Lora_Task, "Lora_Task", 512, NULL, LORA_PRIORITY, NULL);
  Task_Status &= xTaskCreate(ADC_Task, "ADC_Task", 128, NULL, ADC_PRIORITY, NULL);
#ifdef DEBUG
  Task_Status &= xTaskCreate(Collect_Stats, "Stats_Task", 512, NULL, STATS_PRIORITY, NULL);
#endif

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
  volatile CAN_Frame rFrame;
  volatile CAN_Status Receive;

  const TickType_t CANFrequency = 50; // 20Hz
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while(1) {
    Receive = CAN_Receive(CAN1, &rFrame);
    if (Receive == CAN_OK) {
      switch (rFrame.id)
      {
      case 0x048:
        telemetry.RPM = rFrame.data[0] + rFrame.data[1] << 8;
        telemetry.TPS = rFrame.data[2] + rFrame.data[3] << 8;
        telemetry.FOT = rFrame.data[4] + rFrame.data[5] << 8;
        telemetry.IA = rFrame.data[6] + rFrame.data[7] << 8;
        break;
      case 0x148:
        telemetry.Lam = rFrame.data[4] + rFrame.data[5] << 8;
        break;
      case 0x248:
        telemetry.OilP = rFrame.data[6] + rFrame.data[7] << 8;
        break;
      case 0x548:
        telemetry.AirT = rFrame.data[2] + rFrame.data[3] << 8;
        telemetry.CoolT = rFrame.data[4] + rFrame.data[5] << 8;
        break;
      default:
        break;
      }
    }
    vTaskDelayUntil(&xLastWakeTime, CANFrequency);
  }
}

void GPS_Task() {
  GPS_Status status;
  volatile GPS_Data data;
  const TickType_t GPSFrequency = 40; // 25Hz
  vTaskDelay(1000); // Delay for GPS Module to Boot
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
  const TickType_t ADCFrequency = 10; // 100Hz
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while(1) {
    telemetry.FRPot = (ADC_Buffer[0] / ADC_RESOLUTION) * SUS_POT_TRAVEL;
    telemetry.RRPot = (ADC_Buffer[1] / ADC_RESOLUTION) * SUS_POT_TRAVEL;
    telemetry.FRTemp = (ADC_Buffer[2] / ADC_RESOLUTION) * THERMOCOUPLE_CONVERSION;
    telemetry.RRTemp = (ADC_Buffer[3] / ADC_RESOLUTION) * THERMOCOUPLE_CONVERSION;
    telemetry.Steering = (ADC_Buffer[4] / ADC_RESOLUTION) * 360;
    vTaskDelayUntil(&xLastWakeTime, ADCFrequency); 
  }
}

void Lora_Task() {
  LoRa_Status status;

  uint8_t data[] = "Hello World!";
  uint8_t recvData[16];
  uint8_t recLen;

  Clear_Pin(LORA_IO_PORT, LORA_RST);
  vTaskDelay(10);
  Set_Pin(LORA_IO_PORT, LORA_RST);
  vTaskDelay(100);
  status = Lora_Init();

  if (status != LORA_OK) {
    status = Lora_Init();
  }

  const TickType_t LoraFrequency = 50;
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while(1) {
    Lora_Transmit(data, sizeof(data));
    // Lora_Receive(recvData, recLen);
    vTaskDelayUntil(&xLastWakeTime, LoraFrequency);
  }
}

#ifdef DEBUG
void Collect_Stats() {
  const TickType_t StatsFrequency = 1000;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  uint8_t StatsBuffer[64*5];

  while(1) {
    vTaskGetRunTimeStats(&StatsBuffer);
    send_String(USART3, &StatsBuffer);
    vTaskDelayUntil(&xLastWakeTime, StatsFrequency);
  }
}
#endif

void Error_Handler() {
  Set_Pin(GPIOC, STATUS_LED_PIN);
  while(1);
}

void EXTI9_5_IRQHandler() {
  if (EXTI->PR & (0x1 << 9)) {
    EXTI->PR |= (0x1 << 9); // Clear the status bit
    // Set Flag for Lora Recv
  }
}