/************************************************
* @file    spi.c 
* @author  APBashara
* @date    6/2024
* 
* @brief   SPI Driver Implementation
***********************************************/

#include "spi.h"

void SPI2_Init() {
  // Enable Clocks
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

  // GPIO Configuration
  GPIOB->MODER &= ~GPIO_MODER_MODE12 & ~GPIO_MODER_MODE13
               & ~GPIO_MODER_MODE14 & ~GPIO_MODER_MODE15;
  GPIOB->MODER |= (0x01 << GPIO_MODER_MODE12_Pos) | (0x02 << GPIO_MODER_MODE13_Pos)
               | (0x02 << GPIO_MODER_MODE14_Pos) | (0x02 << GPIO_MODER_MODE15_Pos);
  GPIOB->AFR[1] |= (0x05 << GPIO_AFRH_AFSEL13_Pos) | (0x05 << GPIO_AFRH_AFSEL14_Pos) 
                | (0x05 << GPIO_AFRH_AFSEL15_Pos);
  GPIOB->OTYPER &= ~GPIO_MODER_MODE12 & ~GPIO_MODER_MODE13
                & ~GPIO_MODER_MODE14 & ~GPIO_MODER_MODE15;
  GPIOB->OSPEEDR |= (0x02 << GPIO_OSPEEDR_OSPEED12_Pos) | (0x03 << GPIO_OSPEEDR_OSPEED13_Pos)
                 | (0x03 << GPIO_OSPEEDR_OSPEED14_Pos) | (0x03 << GPIO_OSPEEDR_OSPEED15_Pos);
  GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD12 & ~GPIO_PUPDR_PUPD13
               & ~GPIO_PUPDR_PUPD14 & ~GPIO_PUPDR_PUPD15;
  GPIOB->PUPDR |= (0x01 << GPIO_PUPDR_PUPD14_Pos);

  // Configure SPI
  SPI2->CR1 &= ~SPI_CR1_BIDIMODE & ~SPI_CR1_CRCEN & ~SPI_CR1_DFF & ~SPI_CR1_RXONLY
            & ~SPI_CR1_SSM & ~SPI_CR1_LSBFIRST & ~SPI_CR1_CPHA & ~SPI_CR1_CPOL;
  SPI2->CR1 |= SPI_CR1_MSTR | (0x3 << SPI_CR1_BR_Pos);
  SPI2->CR2 &= ~SPI_CR2_FRF;
  SPI2->CR2 |= SPI_CR2_SSOE;

  // Set NSS High
  GPIOB->BSRR = GPIO_BSRR_BS12;
}

SPI_Status SPI_Transmit(SPI_TypeDef* SPI, uint8_t* data, size_t len) {
  SPI->CR1 |= SPI_CR1_SPE; // Enable SPI
  if (SPI->CR1 & SPI_CR1_DFF) { // 16-bit Data Frame
    while (len > 0) {
      while (!(SPI->SR & SPI_SR_TXE)); 
      SPI->DR = *((uint16_t*)data);
      // increment data pointer by 2 bytes
      data += sizeof(uint16_t);
      len--;
    }
  }
  else { // 8-bit Data Frame
    while (len > 0) {
      while (!(SPI->SR & SPI_SR_TXE));
      SPI->DR = *data;
      data++;
      len--;
    }
  }
  // Wait for last byte to be sent
  while (!(SPI->SR & SPI_SR_TXE));
  (void) SPI->DR; // Clear RXNE flag
  (void) SPI->SR; // Clear OVR flag
  while (SPI->SR & SPI_SR_BSY);

  SPI->CR1 &= ~SPI_CR1_SPE; // Disable SPI

  return SPI_OK;
}

SPI_Status SPI_Receive(SPI_TypeDef* SPI, uint8_t* buf, size_t len) {
  SPI->CR1 |= SPI_CR1_SPE; // Enable SPI
  if (SPI->CR1 & SPI_CR1_DFF) { // 16-bit Data Frame
    while (len > 0) {
      while(!(SPI->SR & SPI_SR_TXE));
      SPI->DR = 0x0000; // Dummy Data
      while (!(SPI->SR & SPI_SR_RXNE));
      *((uint16_t*)buf) = SPI->DR; // Cast to 16-bit
      // increment data pointer by 2 bytes and len by 2
      buf += sizeof(uint16_t);
      len --;
    }
  }
  else { // 8-bit Data Frame
    for (size_t i = 0; i < len; i++) {
      while(!(SPI->SR & SPI_SR_TXE));
      SPI->DR = 0x00; // Dummy Data to generate clk
      while (!(SPI->SR & SPI_SR_RXNE));
      buf[i] = SPI->DR;
    }
  }
  // Wait for last byte to be received
  while (SPI->SR & SPI_SR_BSY);

  SPI->CR1 &= ~SPI_CR1_SPE; // Disable SPI

  return SPI_OK;
}

