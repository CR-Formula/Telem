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
  GPIOB->OSPEEDR |= (0x03 << GPIO_OSPEEDR_OSPEED12_Pos) | (0x03 << GPIO_OSPEEDR_OSPEED13_Pos)
                 | (0x03 << GPIO_OSPEEDR_OSPEED14_Pos) | (0x03 << GPIO_OSPEEDR_OSPEED15_Pos);
  GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD12 & ~GPIO_PUPDR_PUPD13
               & ~GPIO_PUPDR_PUPD14 & ~GPIO_PUPDR_PUPD15;

  // Configure SPI
  SPI2->CR1 &= ~SPI_CR1_BIDIMODE & ~SPI_CR1_CRCEN & ~SPI_CR1_DFF & ~SPI_CR1_RXONLY
            & ~SPI_CR1_SSM & ~SPI_CR1_LSBFIRST & ~SPI_CR1_CPHA & ~SPI_CR1_CPOL;
  SPI2->CR1 |= SPI_CR1_MSTR | (0x5 << SPI_CR1_BR_Pos);
  SPI2->CR2 &= ~SPI_CR2_FRF;
  SPI2->CR2 |= SPI_CR2_SSOE;

  // Enable SPI
  SPI2->CR1 |= SPI_CR1_SPE;
  GPIOB->BSRR = GPIO_BSRR_BS12; // Set NSS High
}

SPI_Status SPI_Transmit(SPI_TypeDef* SPI, uint8_t* data, size_t len) {
  if (SPI->CR1 & SPI_CR1_DFF) { // 16-bit Data Frame
    while (len > 0) {
      while (!(SPI->SR & SPI_SR_TXE)); // Wait until TXE is set
      SPI->DR = *((uint16_t*)data); // Write data to DR
      // increment data pointer by 2 bytes
      data += sizeof(uint16_t);
      len--;
    }
  }
  else { // 8-bit Data Frame
    while (len > 0) {
      while (!(SPI->SR & SPI_SR_TXE));
      SPI->DR = *data;
      data += sizeof(uint8_t);
      len--;
    }
  }
  // Wait for last byte to be sent
    while (!(SPI->SR & SPI_SR_TXE));
    while (SPI->SR & SPI_SR_BSY);
  return SPI_OK;
}

SPI_Status SPI_Receive(SPI_TypeDef* SPI, uint8_t* buf, size_t len) {
  if (SPI->CR1 & SPI_CR1_DFF) { // 16-bit Data Frame
    while (len > 0) {
      SPI->DR = 0x0000; // Write dummy data to DR
      while (!(SPI->SR & SPI_SR_RXNE)); // Wait until RXNE is set
      *((uint16_t*)buf) = SPI->DR; // Read data from DR
      // increment data pointer by 2 bytes and len by 2
      buf += sizeof(uint16_t);
      len --;
    }
  }
  else { // 8-bit Data Frame
    while (len > 0) {
      SPI->DR = 0x00; // Write dummy data to DR
      while (!(SPI->SR & SPI_SR_RXNE));
      *buf = SPI->DR;
      buf += sizeof(uint8_t);
      len--;
    }
  }
  // Wait for last byte to be received
  while (SPI->SR & SPI_SR_BSY);
  return SPI_OK;
}

