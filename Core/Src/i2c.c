/************************************************
* @file    i2c.c 
* @author  APBashara
* @date    6/2024
* 
* @brief   I2C Driver Implementation
***********************************************/

#include "i2c.h"

/**
 * @brief Generate I2C Start Condition
 * 
 * @param I2C [I2C_TypeDef*] Peripheral to use
 */
static void __Start(I2C_TypeDef* I2C) {
    I2C->CR1 |= I2C_CR1_START;
    while (!(I2C->SR1 & I2C_SR1_SB)); // Wait for start bit to be set
    (void) I2C->SR1; // Clear start bit by reading SR1
}

/**
 * @brief Initialize I2C1
 * @note FM, 400kHz, 7-bit Addressing
 */
I2C_Status I2C1_Init() {
    // Enable I2C1 and GPIOB Clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // Configure GPIOB for I2C1 (PB6 - SCL, PB7 - SDA)
    GPIOB->MODER &= ~(GPIO_MODER_MODE6_Msk) & ~(GPIO_MODER_MODE7_Msk);
    GPIOB->MODER |= (0x2 << GPIO_MODER_MODE6_Pos) | (0x2 << GPIO_MODER_MODE7_Pos);
    GPIOB->AFR[0] &= ~((0xF<<GPIO_AFRL_AFSEL6_Pos)|(0xF<<GPIO_AFRL_AFSEL7_Pos));
    GPIOB->AFR[0] |= (0x4 << GPIO_AFRL_AFSEL6_Pos) | (0x4 << GPIO_AFRL_AFSEL7_Pos);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD6_Msk) & ~(GPIO_PUPDR_PUPD7_Msk); // Using external pull-up resistors
    GPIOB->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED6_Msk) & ~(GPIO_OSPEEDR_OSPEED7_Msk);
    GPIOB->OSPEEDR |= (0x3 << GPIO_OSPEEDR_OSPEED6_Pos) | (0x3 << GPIO_OSPEEDR_OSPEED7_Pos);
    GPIOB->OTYPER |= (GPIO_OTYPER_OT6) | (GPIO_OTYPER_OT7);

    // Reset and then clear reset of I2C
    I2C1->CR1 |= I2C_CR1_SWRST;
    I2C1->CR1 &= ~I2C_CR1_SWRST;

    // Disable I2C before configuring it
    I2C1->CR1 &= ~I2C_CR1_PE;

    // Configure I2C1
    I2C1->CR2 = (42u << I2C_CR2_FREQ_Pos); // Set peripheral clock frequency (42 MHz)

    // Configure clock control register for 400kHz I2C speed
    // https://www.teachmemicro.com/stm32-i2c-calculator/
    uint16_t ccr_value = 35; // Fast mode, duty cycle 16/9
    I2C1->CCR = I2C_CCR_FS | ccr_value;

    // Configure maximum rise time
    uint8_t trise = 14;
    I2C1->TRISE = trise;

    // Enable I2C
    I2C1->CR1 |= I2C_CR1_ACK | I2C_CR1_PE;

    return I2C_OK;
}

void I2C_Write(I2C_TypeDef* I2C, uint8_t dev, uint8_t* data, const size_t len) {
    
    while (I2C->SR2 & I2C_SR2_BUSY); // Wait for I2C to be ready
    __Start(I2C);

    // Send address
    I2C->DR = (dev << 1) & ~0x1; // Send address with write bit
    while (!(I2C->SR1 & I2C_SR1_ADDR)); // Wait for address to be sent
    (void) I2C->SR1; (void) I2C->SR2; // Clear address flag 

    for (size_t i = 0; i < len; i++) {
        // Send data
        I2C->DR = data[i];
        while (!(I2C->SR1 & I2C_SR1_TXE)); // Wait for data register to empty
    }

    while (!(I2C1->SR1 & I2C_SR1_BTF)); // Wait for transfer to complete
    I2C->CR1 |= I2C_CR1_STOP;
}

I2C_Status I2C_Read(I2C_TypeDef* I2C, uint8_t dev, uint8_t reg, uint8_t* data, const size_t len) {

    while (I2C->SR2 & I2C_SR2_BUSY);
    __Start(I2C);

    // Send dev address
    I2C->DR = (dev << 1) & ~0x1; // Send address with read bit
    while (!(I2C->SR1 & I2C_SR1_ADDR)); // Wait for address to be sent
    (void) I2C->SR1; (void) I2C->SR2; // Clear address flag

    I2C->DR = reg; // Send register address
    while (!(I2C->SR1 & I2C_SR1_TXE)); // Wait for data register to empty

    __Start(I2C); // Repeated start

    I2C->DR = (dev << 1) | 0x1; // Send address with read bit
    while (!(I2C->SR1 & I2C_SR1_ADDR)); // Wait for address to be sent
    if (len == 1) {
        I2C->CR1 &= ~I2C_CR1_ACK; // Disable ACK
    }
    else if (len == 2) {
        I2C->CR1 &= ~I2C_CR1_ACK; // Disable ACK
        I2C->CR1 |= I2C_CR1_POS; // Set position for NACK
    }
    (void) I2C->SR1; (void) I2C->SR2; // Clear address flag

    if (len == 1) {
        I2C->CR1 |= I2C_CR1_STOP; // Send stop condition
        while (!(I2C->SR1 & I2C_SR1_RXNE)); // Wait for data to be received
        data[0] = I2C->DR; // Read data
    }

    if (len == 2) {
        while (!(I2C->SR1 & I2C_SR1_BTF)); // Wait for transfer to complete
        I2C->CR1 |= I2C_CR1_STOP; // Send stop condition
        data[0] = I2C->DR; // Read first byte
        while (!(I2C->SR1 & I2C_SR1_RXNE)); // Wait for data to be received
        data[1] = I2C->DR; // Read second byte
    }

    if (len > 2) {
        for (size_t i = 0; i < len - 3; i++) {
            while (!(I2C->SR1 & I2C_SR1_RXNE)); // Wait for data to be received
            data[i] = I2C->DR; // Read data
        }
        while (!(I2C->SR1 & I2C_SR1_BTF)); // Wait for transfer to complete
        I2C->CR1 &= ~I2C_CR1_ACK; // Disable ACK
        data[len - 3] = I2C->DR; // Read last byte
        while (!(I2C->SR1 & I2C_SR1_BTF));
        I2C->CR1 |= I2C_CR1_STOP; // Send stop condition
        data[len - 2] = I2C->DR;
        while (!(I2C->SR1 & I2C_SR1_RXNE));
        data[len - 1] = I2C->DR;
    }

    I2C->CR1 |= I2C_CR1_ACK; // Enable ACK
    return I2C_OK;
}