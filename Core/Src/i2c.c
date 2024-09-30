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
void __Start(I2C_TypeDef* I2C) {
    I2C->CR1 |= I2C_CR1_START;
    while (!(I2C->SR1 & I2C_SR1_SB)); // Wait for start bit to be set
}

/**
 * @brief Generate I2C Stop Condition
 * 
 * @param I2C [I2C_TypeDef*] Peripheral to use
 */
void __Stop(I2C_TypeDef* I2C) {
    I2C->CR1 |= I2C_CR1_STOP;
    while(!(I2C1->SR2 & I2C_SR2_BUSY));
}

/**
 * @brief Initialize I2C1
 * @note FM, 400kHz, 7-bit Addressing
 */
I2C_Status I2C1_Init() {
    // Enable I2C1 and GPIOB Clocks
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    // Reset and then clear reset of I2C
    I2C1->CR1 |= I2C_CR1_SWRST;
    I2C1->CR1 &= ~I2C_CR1_SWRST;

    // Disable I2C before configuring it
    I2C1->CR1 &= ~I2C_CR1_PE;

    // Configure GPIOB for I2C1 (PB6 - SCL, PB7 - SDA)
    GPIOB->MODER &= ~(GPIO_MODER_MODE6_Msk) & ~(GPIO_MODER_MODE7_Msk);
    GPIOB->MODER |= (0x2 << GPIO_MODER_MODE6_Pos) | (0x2 << GPIO_MODER_MODE7_Pos);
    GPIOB->AFR[0] |= (0x4 << GPIO_AFRL_AFSEL6_Pos) | (0x4 << GPIO_AFRL_AFSEL7_Pos);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD6_Msk) & ~(GPIO_PUPDR_PUPD7_Msk);
    GPIOB->PUPDR |= (0x1 << GPIO_PUPDR_PUPD6_Pos) | (0x1 << GPIO_PUPDR_PUPD7_Pos);
    GPIOB->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED6_Msk) & ~(GPIO_OSPEEDR_OSPEED7_Msk);
    GPIOB->OSPEEDR |= (0x3 << GPIO_OSPEEDR_OSPEED6_Pos) | (0x3 << GPIO_OSPEEDR_OSPEED7_Pos);
    GPIOB->OTYPER |= (GPIO_OTYPER_OT6) | (GPIO_OTYPER_OT7);

    // Configure I2C1
    I2C1->CR2 = (42u << I2C_CR2_FREQ_Pos); // Set peripheral clock frequency (42 MHz)

    // Configure clock control register for 400kHz I2C speed
    uint32_t pclk1 = 42000000; // APB1 clock frequency
    uint32_t i2c_speed = 400000; // I2C speed
    uint32_t ccr_value = (pclk1 / (25 * i2c_speed)); // Fast mode, duty cycle 16/9
    I2C1->CCR = I2C_CCR_FS | I2C_CCR_DUTY | ccr_value;

    // Configure maximum rise time
    uint32_t trise = ((pclk1 / 3000) + 1); // Fast mode max rise time
    I2C1->TRISE = trise;

    // Enable I2C
    I2C1->CR1 |= I2C_CR1_PE;
    I2C1->CR1 |= I2C_CR1_ACK; // Enable ACK
}

void I2C_Write(I2C_TypeDef* I2C, uint8_t dev, uint8_t* data, size_t len) {
    
    __Start(I2C);

    // Send address
    I2C->DR = (dev << 1) & 0xFE; // Send address with write bit
    while (!(I2C->SR1 & I2C_SR1_ADDR)); // Wait for address to be sent
    (void) I2C->SR2; // Clear address flag by reading SR1 and SR2

    for (size_t i = 0; i < len; i++) {
        // Send data
        I2C->DR = data[i];
        while (!(I2C->SR1 & I2C_SR1_TXE)); // Wait for data register to empty
    }

    __Stop(I2C);
}

/**
 * @brief Read a byte of data over I2C
 * 
 * @param I2C [I2C_TypeDef*] Peripheral to use
 * @param dev [uint8_t] Address of device [7-bit]
 * @param reg [uint8_t] Register to read
 */
uint8_t I2C_Read_Reg(I2C_TypeDef* I2C, uint8_t dev, uint8_t reg) {
    uint8_t data = 0;

    __Start(I2C);

    // Send address
    I2C->DR = (dev << 1) & 0xFE; // Send address with write bit
    while (!(I2C->SR1 & I2C_SR1_ADDR)); // Wait for address to be sent
    (void) I2C->SR2; // Clear address flag by reading SR1 and SR2

    // Send register
    I2C->DR = reg;
    while (!(I2C->SR1 & I2C_SR1_TXE)); // Wait for data register to empty

    __Stop(I2C);
    __Start(I2C);

    // Send address
    I2C->DR = (dev << 1) | 0x01; // Send address with read bit
    while (!(I2C->SR1 & I2C_SR1_ADDR)); // Wait for address to be sent
    (void) I2C->SR2; // Clear address flag by reading SR1 and SR2

    // Read data
    while (!(I2C->SR1 & I2C_SR1_RXNE)); // Wait for data to be received
    data = (uint8_t)I2C->DR;

    __Stop(I2C);

    return data;
}

I2C_Status I2C_Read(I2C_TypeDef* I2C, uint8_t dev, uint8_t* data, size_t len) {
    I2C->CR1 &= ~I2C_CR1_POS; // Disable POS
    I2C->CR1 |= I2C_CR1_ACK; // Enable ACK
    __Start(I2C);

    // Send address
    I2C->DR = (dev << 1) | 0x01; // Send address with read bit
    while (!(I2C->SR1 & I2C_SR1_ADDR)); // Wait for address to be sent
    (void) I2C->SR2; // Clear address flag by reading SR1 and SR2

    for (size_t i = 0; i < len; i++) {
        // Read data
        while ((I2C->SR1 & I2C_SR1_TXE)); // Wait for data to be received
        data[i] = (uint8_t)I2C->DR;
        while (!(I2C->SR1 & I2C_SR1_BTF)); // wait for byte transfer to finish
    }

    __Stop(I2C);

    return I2C_OK;
}