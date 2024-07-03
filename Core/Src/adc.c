/************************************************
* @file    adc.c
* @author  APBashara
* @date    6/2024
* 
* @brief   Basic ADC Driver
***********************************************/

#include "stm32f407xx.h"

/**
 * @brief Initialize ADC1
 * @note Sets up 16 conversions for all 16 channels
 * @note Ideally used with DMA for continuous conversion
 */
void ADC_Init() {
    ADC->CCR |= (0x1 << ADC_CCR_ADCPRE_Pos); // Set ADC Prescaler to 4 (84MHz / 4 = 21MHz)
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // Enable ADC1 Clock
    // Enable GPIO Clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;

    // Page 272 for GPIO Configuration
    // Set to Analog Mode and disable Pull-up Pull-down
    GPIOA->MODER |= (0x3 << GPIO_MODER_MODE0_Pos) | (0x3 << GPIO_MODER_MODE1_Pos)
                 | (0x3 << GPIO_MODER_MODE4_Pos) | (0x3 << GPIO_MODER_MODE5_Pos)
                 | (0x3 << GPIO_MODER_MODE6_Pos) | (0x3 << GPIO_MODER_MODE7_Pos);
    GPIOB->MODER |= (0x3 << GPIO_MODER_MODE8_Pos) | (0x3 << GPIO_MODER_MODE9_Pos);
    GPIOC->MODER |= (0x3 << GPIO_MODER_MODE0_Pos) | (0x3 << GPIO_MODER_MODE1_Pos)
                 | (0x3 << GPIO_MODER_MODE2_Pos) | (0x3 << GPIO_MODER_MODE3_Pos)
                 | (0x3 << GPIO_MODER_MODE4_Pos) | (0x3 << GPIO_MODER_MODE5_Pos);
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR0 & ~GPIO_PUPDR_PUPDR1
                 & ~GPIO_PUPDR_PUPDR4 & ~GPIO_PUPDR_PUPDR5
                 & ~GPIO_PUPDR_PUPDR6 & ~GPIO_PUPDR_PUPDR7;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR8 & ~GPIO_PUPDR_PUPDR9;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR0 & ~GPIO_PUPDR_PUPDR1
                 & ~GPIO_PUPDR_PUPDR2 & ~GPIO_PUPDR_PUPDR3
                 & ~GPIO_PUPDR_PUPDR4 & ~GPIO_PUPDR_PUPDR5;

    ADC1->SQR1 |= (0xF << ADC_SQR1_L_Pos); // Set Regular Sequence Length to 16
    
    // Set ADC Conversion Sequence
    ADC1->SQR3 |= (0x0 << ADC_SQR3_SQ1_Pos) | (0x1 << ADC_SQR3_SQ2_Pos)
                | (0x2 << ADC_SQR3_SQ3_Pos) | (0x3 << ADC_SQR3_SQ4_Pos)
                | (0x4 << ADC_SQR3_SQ5_Pos) | (0x5 << ADC_SQR3_SQ6_Pos);
    ADC1->SQR2 |= (0x6 << ADC_SQR2_SQ7_Pos) | (0x7 << ADC_SQR2_SQ8_Pos)
                | (0x8 << ADC_SQR2_SQ9_Pos) | (0x9 << ADC_SQR2_SQ10_Pos)
                | (0xA << ADC_SQR2_SQ11_Pos) | (0xB << ADC_SQR2_SQ12_Pos);
    ADC1->SQR1 |= (0xC << ADC_SQR1_SQ13_Pos) | (0xD << ADC_SQR1_SQ14_Pos)
                | (0xE << ADC_SQR1_SQ15_Pos) | (0xF << ADC_SQR1_SQ16_Pos);


    ADC1->CR1 |= ADC_CR1_SCAN; // Enable Scan Mode
    ADC1->CR2 |= ADC_CR2_EOCS; // Enable End of Conversion Selection
    ADC1->CR2 |= ADC_CR2_ADON; // Enable ADC
}

/**
 * @brief Initalize the DMA2 for ADC1
 * 
 * @note Sets up the DMA to transfer 16 16-bit values from ADC1->DR to buffer
 * @param buffer [uint16_t] Buffer[16] to store ADC values
 */
void DMA_ADC1_Init(uint16_t* buffer) {
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN; // Enable DMA2 Clock

    // Make sure that the ADC is configured before enabling DMA
    ADC1->CR2 &= ~ADC_CR2_ADON; // Disable ADC
    ADC1->CR2 |= ADC_CR2_DMA; // Enable DMA for ADC1
    ADC1->CR2 |= ADC_CR2_CONT; // Enable Continuous Conversion Mode
    ADC1->CR2 |= ADC_CR2_DDS; // Enable DMA Request after last transfer
    ADC1->CR2 &= ~ADC_CR2_EOCS; // Enable End of Conversion Selection
    ADC1->CR2 |= ADC_CR2_ADON; // Enable ADC

    DMA2_Stream0->CR &= ~DMA_SxCR_EN; // Disable DMA Stream 0
    while (DMA2_Stream0->CR & DMA_SxCR_EN); // Wait for Stream to be Disabled

    DMA2_Stream0->PAR = (uint32_t) &(ADC1->DR); // Set Peripheral Address to ADC1 Data Register
    DMA2_Stream0->M0AR = (uint32_t) buffer; // Set Memory 0 Address to buffer

    DMA2_Stream0->NDTR = 16; // Set Number of Data to Transfer to 16


    DMA2_Stream0->CR |= (0x00 << DMA_SxCR_CHSEL_Pos) // Set Channel to 0
                     | (0x3 << DMA_SxCR_PL_Pos) // Set Priority to Very High
                     | (0x00 << DMA_SxCR_DIR_Pos) // Set Direction to Peripheral to Memory
                     | (0x1 << DMA_SxCR_MSIZE_Pos) // Set Memory Size to 16-bits
                     | (0x1 << DMA_SxCR_PSIZE_Pos) // Set Peripheral Size to 16-bits
                     | (0x1 << DMA_SxCR_MINC_Pos) // Enable Memory Increment Mode
                     | (0x1 << DMA_SxCR_CIRC_Pos) // Enable Circular Mode
                     | (0x3 << DMA_SxCR_PBURST_Pos) // Set Peripheral Burst to 16 beats
                     | (0x3 << DMA_SxCR_MBURST_Pos); // Set Memory Burst to 16 beats

    DMA2_Stream0->CR |= DMA_SxCR_EN; // Enable DMA Stream 0
    ADC1->CR2 |= ADC_CR2_SWSTART; // Start Conversion
}

/**
 * @brief Read ADC PA1
 * 
 * @param adc_value [uint16_t*] Pointer to store ADC value
 */
void ADC_Read(uint16_t *adc_value) {
    ADC1->CR2 |= ADC_CR2_SWSTART; // Start Conversion

    for (int i = 0; i < 3; i++) {
      while (!(ADC1->SR & ADC_SR_EOC)); // Wait for End of Conversion
      adc_value[i] = ADC1->DR; // Return the Data Register
    }
}