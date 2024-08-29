#include "stm32f415xx.h"
#include "cmsis_os2.h"

void Status_LED(void *argument);

osThreadId_t StatusLED;
const osThreadAttr_t StatusLED_attr = {
  .name = "Status_Task",
  .priority = osPriorityNormal,
  .stack_size = 128
};

int main() {
  osKernelInitialize(); // Initialize FreeRTOS
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
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
  GPIOC->MODER |= GPIO_MODER_MODE13_0;
  GPIOC->OTYPER &= ~GPIO_OTYPER_OT13;
  GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED13;

  while(1) {
    osDelay(1000);
    GPIOC->ODR ^= GPIO_ODR_OD13;
  }
}