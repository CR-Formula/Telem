#include "stm32f415xx.h"

void static inline Toggle_Pin(GPIO_TypeDef* GPIO, int pin) {
  GPIO->ODR ^= (1 << pin);
}