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
SemaphoreHandle_t LoRa_Mutex;
uint16_t ADC_Buffer[16];

// Task Handlers
TaskHandle_t xCAN_Task;

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

  // Create Tasks to collect Data
  Task_Status &= xTaskCreate(ADC_Task, "ADC_Task", 128, NULL, ADC_PRIORITY, NULL);
  // Task_Status &= xTaskCreate(GPS_Task, "GPS_Task", 512, NULL, GPS_PRIORITY, NULL);
  Task_Status &= xTaskCreate(CAN_Task, "CAN_Task", 256, NULL, CAN_PRIORITY, &xCAN_Task);
  Task_Status &= xTaskCreate(Status_LED, "Status_Task", 128, NULL, LED_PRIORITY, NULL);
#ifdef STATS
  Task_Status &= xTaskCreate(Collect_Stats, "Stats_Task", 512, NULL, STATS_PRIORITY, NULL);
#endif

  // Create Tasks to send LoRa Packets
  Task_Status &= xTaskCreate(LoRa_Suspension_Task, "LoRa_Suspension_Task", 128, NULL, LORA_PRIORITY, NULL);
  Task_Status &= xTaskCreate(LoRa_GPS_Task, "LoRa_GPS_Task", 128, NULL, LORA_PRIORITY, NULL);
  Task_Status &= xTaskCreate(LoRa_Engine_Data_Task, "LoRa_Engine_Data_Task", 128, NULL, LORA_PRIORITY, NULL);
  Task_Status &= xTaskCreate(LoRa_Brakes_Accel_Task, "LoRa_Brakes_Accel_Task", 128, NULL, LORA_PRIORITY, NULL);
  Task_Status &= xTaskCreate(LoRa_Temperature_Task, "LoRa_Temperature_Task", 128, NULL, LORA_PRIORITY, NULL);

  
  // Check that tasks were created successfully
  if (Task_Status != pdPASS) {
    Error_Handler();
  }

  // Create and check LoRa Mutex Creation
  LoRa_Mutex = xSemaphoreCreateMutex();
  if (LoRa_Mutex == NULL) {
    Error_Handler();
  }

  vTaskStartScheduler(); // Start FreeRTOS Scheduler

  while(1);
}

/* Telemetry Tasks ----------------------------------------------------------*/
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
        telemetry.ThrottlePosSensor = rFrame.data[2] + rFrame.data[3] << 8;
        break;
      case 0x148:
        telemetry.Lambda = rFrame.data[4] + rFrame.data[5] << 8;
        break;
      case 0x248:
        telemetry.OilPressure = rFrame.data[6] + rFrame.data[7] << 8;
        break;
      case 0x548:
        telemetry.AirTemp = rFrame.data[2] + rFrame.data[3] << 8;
        telemetry.CoolTemp = rFrame.data[4] + rFrame.data[5] << 8;
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
    telemetry.FrontPot = (ADC_Buffer[Sus_Pot_1_ADC] / ADC_RESOLUTION) * SUS_POT_TRAVEL;
    telemetry.RearPot = (ADC_Buffer[Sus_Pot_2_ADC] / ADC_RESOLUTION) * SUS_POT_TRAVEL;
    telemetry.Steering = (ADC_Buffer[Steering_Angle_ADC] / ADC_RESOLUTION) * 360;
    telemetry.BrakePressure = (ADC_Buffer[Brake_Position_ADC] / ADC_RESOLUTION) * 100;
    vTaskDelayUntil(&xLastWakeTime, ADCFrequency); 
  }
}

#ifdef STATS
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

/* LoRa Transmit Tasks ------------------------------------------------------*/
void LoRa_Suspension_Task() {
  const TickType_t LoRaFrequency = 20; // 50Hz
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while(1) {
    if (xSemaphoreTake(LoRa_Mutex, portMAX_DELAY) == pdTRUE) {
      Lora_Transmit(&telemetry, LORA_SUSPENSION_SIZE);
      xSemaphoreGive(LoRa_Mutex);
    }
    vTaskDelayUntil(&xLastWakeTime, LoRaFrequency); // 50Hz rate = 20ms period
  }
}

void LoRa_GPS_Task() {
  const TickType_t LoRaFrequency = 40; // 25Hz
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while(1) {
    if (xSemaphoreTake(LoRa_Mutex, portMAX_DELAY) == pdTRUE) {
      Lora_Transmit(&telemetry.latGPS, LORA_GPS_SIZE);
      xSemaphoreGive(LoRa_Mutex);
    }
    vTaskDelayUntil(&xLastWakeTime, LoRaFrequency); // 25Hz rate = 40ms period
  }
}

void LoRa_Engine_Data_Task() {
  const TickType_t LoRaFrequency = 50; // 20Hz
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while(1) {
    if (xSemaphoreTake(LoRa_Mutex, portMAX_DELAY) == pdTRUE) {
      Lora_Transmit(&telemetry.BrakePressure, LORA_ENGINE_SIZE);
      xSemaphoreGive(LoRa_Mutex);
    }
    vTaskDelayUntil(&xLastWakeTime, LoRaFrequency); // 20Hz rate = 50ms period
  }
}

void LoRa_Brakes_Accel_Task() {
  const TickType_t LoRaFrequency = 100; // 10Hz
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while(1) {
    if (xSemaphoreTake(LoRa_Mutex, portMAX_DELAY) == pdTRUE) {
      Lora_Transmit(&telemetry.OilPressure, LORA_BRAKES_ACCEL_SIZE);
      xSemaphoreGive(LoRa_Mutex);
    }
    vTaskDelayUntil(&xLastWakeTime, LoRaFrequency); // 10Hz rate = 100ms period
  }
}

void LoRa_Temperature_Task() {
  const TickType_t LoRaFrequency = 1000; // 1Hz
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while(1) {
    if (xSemaphoreTake(LoRa_Mutex, portMAX_DELAY) == pdTRUE) {
      Lora_Transmit(&telemetry.AirTemp, LORA_TEMPERATURE_SIZE);
      xSemaphoreGive(LoRa_Mutex);
    }
    vTaskDelayUntil(&xLastWakeTime, LoRaFrequency); // 1Hz rate = 1000ms period
  }
}


/* Error Handlers -----------------------------------------------------------*/
void Error_Handler() {
  Set_Pin(GPIOC, STATUS_LED_PIN);
  while(1);
}

/* Interrupt Handlers -------------------------------------------------------*/
void EXTI9_5_IRQHandler() {
  if (EXTI->PR & (0x1 << 9)) {
    EXTI->PR |= (0x1 << 9); // Clear the status bit
    // Set Flag for Lora Recv
  }
}