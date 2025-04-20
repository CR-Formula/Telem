/************************************************
* @file    can.h 
* @author  APBashara
* @date    7/2024
* 
* @brief   Prototype Functions for CAN Driver
***********************************************/

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "stm32f415xx.h"

extern TaskHandle_t xCAN_Task;
extern QueueHandle_t canRXQueue;

typedef enum {
    CAN_RTR_Data,
    CAN_RTR_Remote
} CAN_RTR;

typedef enum {
    CAN_OK,
    CAN_TX_Req,
    CAN_Error,
    CAN_Mailbox_Error,
    CAN_Fifo_Error
} CAN_Status;

typedef enum {
    CAN_State_Sleep,
    CAN_State_Initialization,
    CAN_State_Normal,
} CAN_State;

typedef struct {
    uint16_t id; // 11-bit ID
    uint8_t dlc; // Data Length Code
    CAN_RTR rtr; // Remote Transmission Request
    uint8_t data[8]; // Data Bytes
} CAN_Frame;

/**
 * @brief Initializes CAN1
 * 
 * @note Pins PA11(Rx) and PA12 (Tx)
 * @note Baud Rate: 500kbps
 * @return CAN_Status
 */
CAN_Status CAN1_Init();

/**
 * @brief Initalize CAN Filters
 * 
 * @return CAN_Status 
 */
CAN_Status CAN_Filters_Init();

/**
 * @brief Start communication on the CAN Bus
 * 
 * @return CAN_Status 
 */
CAN_Status CAN_Start();

/**
 * @brief Stop Communication on the CAN Bus
 * 
 * @return CAN_Status 
 */
CAN_Status CAN_Stop();

/**
 * @brief Add a CAN frame to the transmit mailbox
 * 
 * @param CAN [CAN_TypeDef*] CAN Peripheral to receive from
 * @param frame [CAN_Frame*] Frame to transmit
 * @return [CAN_Status] Status of Transmission
 */
CAN_Status CAN_Transmit(CAN_TypeDef* CAN, CAN_Frame* frame);

/**
 * @brief Receive a CAN Frame
 * 
 * @param CAN [CAN_TypeDef*] CAN Peripheral to receive from
 * @param frame [CAN_Frame*] Frame struct to receive
 * @return [CAN_Status] Status of Reception
 */
CAN_Status CAN_Receive(CAN_TypeDef* CAN, CAN_Frame* frame);