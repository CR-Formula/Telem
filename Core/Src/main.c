#include "stm32f415xx.h"
#include "cmsis_os2.h"

#include <stdint.h>

#include "gpio.h"

void Status_LED(void *argument);

osThreadId_t StatusLED;
const osThreadAttr_t StatusLED_attr = {
  .name = "Status_Task",
  .priority = osPriorityBelowNormal,
  .stack_size = 128
};

int main() {
  osKernelInitialize(); // Initialize FreeRTOS

  LED_Init();

  StatusLED = osThreadNew(Status_LED, NULL, &StatusLED_attr);

  osKernelStart(); // Start FreeRTOS
  while(1) {

  }
}

/**
 * @brief Thread for blinking the status led
 * 
 * @param argument 
 */
void Status_LED(void *argument) {
  while(1) {
    osDelay(1000);
    Toggle_Pin(GPIOC, 13);
  }
}