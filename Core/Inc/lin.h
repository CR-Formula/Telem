/************************************************
* @file    lin.h 
* @author  APBashara
* @date    9/2024
* 
* @brief   Prototype Functions for LIN Driver
***********************************************/

#include "stm32f415xx.h"

/* Typedefs -----------------------------------------------------------------*/
typedef struct {
    uint8_t ID; // Message ID
    uint8_t data[8]; // Data
} LIN_Message;

typedef enum {
    LIN_Error,
    LIN_OK,
    LIN_Busy,
} LIN_Status;

/* Function Prototypes ------------------------------------------------------*/
/**
 * @brief Initialize LIN peripheral
 * 
 * @return LIN_Status
 */
LIN_Status LIN_Init();

/**
 * @brief Send a packet over LIN Bus
 * 
 * @param USART [USART_TypeDef*] USART to use, must be in LIN mode
 * @param msg [LIN_Message*] Message to send
 * @return LIN_Status
 */
LIN_Status LIN_Transmit(USART_TypeDef* USART, LIN_Message* msg);