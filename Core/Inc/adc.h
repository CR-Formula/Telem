/************************************************
* @file    adc.h 
* @author  APBashara
* @date    6/2024
* 
* @brief   ADC Function Prototypes
***********************************************/

#include "stm32f415xx.h"

#define ADC_RESOLUTION      4096

/**
 * @brief Initialize ADC1
 * @note Sets up 16 conversions for all 16 channels
 * @note Ideally used with DMA for continuous conversion
 */
void ADC_Init();

/**
 * @brief Initalize the DMA2 for ADC1
 * 
 * @note Sets up the DMA to transfer 16 16-bit values from ADC1->DR to buffer
 * @param buffer [uint16_t] Buffer[16] to store ADC values
 */
void DMA_ADC1_Init(uint16_t* buffer);

/**
 * @brief Read ADC PA1
 * 
 * @param adc_value [uint16_t*] Pointer to store ADC value
 */
void ADC_Read(uint16_t *adc_value);