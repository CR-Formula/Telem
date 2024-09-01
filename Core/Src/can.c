/************************************************
* @file    can.c 
* @author  APBashara
* @date    7/2024
* 
* @brief   CAN Driver Implementation
***********************************************/

#include <stddef.h>

#include "stm32f407xx.h"
#include "can.h"

/**
 * @brief Find an empty CAN Transmit mailbox
 * 
 * @return [uint8_t] Index of Empty Mailbox 
 */
static uint8_t Get_Empty_Mailbox() {
    if (CAN1->TSR & CAN_TSR_TME0) {
        return 0u;
    } else if (CAN1->TSR & CAN_TSR_TME1) {
        return 1u;
    } else if (CAN1->TSR & CAN_TSR_TME2) {
        return 2u;
    } else {
        return 0xFFu;
    }
}

/**
 * @brief Initializes CAN1
 * 
 * @note Pins PA11(Rx) and PA12 (Tx)
 * @note Baud Rate: 500kbps
 */
void CAN1_Init() {

    // Setup CAN Clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;

    // Setup CAN GPIO Pins
    GPIOA->MODER &= ~GPIO_MODER_MODE11 & ~GPIO_MODER_MODE12;
    GPIOA->MODER |= (0x2 << GPIO_MODER_MODE11_Pos) | (0x2 << GPIO_MODER_MODE12_Pos);
    GPIOA->AFR[1] |= (0x9 << GPIO_AFRH_AFSEL11_Pos) | (0x9 << GPIO_AFRH_AFSEL12_Pos);
    GPIOA->OSPEEDR |= (0x3 << GPIO_OSPEEDR_OSPEED11_Pos) | (0x3 << GPIO_OSPEEDR_OSPEED12_Pos);

    // Initialize the CAN Peripheral
    CAN1->MCR |= CAN_MCR_INRQ; // Request Initialization Mode
    while (!(CAN1->MSR & CAN_MSR_INAK)); // Wait until Initialization Mode is entered
    CAN1_State = CAN_State_Initialization;

    CAN1->MCR &= ~CAN_MCR_SLEEP; // Exit Sleep Mode
    while (!(CAN1->MSR & CAN_MSR_SLAK)); // Wait until Sleep Mode is exited

    // Configure CAN1 Settings
    CAN1->MCR &= ~CAN_MCR_TXFP & ~CAN_MCR_NART & ~CAN_MCR_RFLM 
                & ~CAN_MCR_TTCM & ~CAN_MCR_ABOM & ~CAN_MCR_TXFP;
    
    // http://www.bittiming.can-wiki.info/
    // Sample Point at 85.7% Reg Value = 0x001a0005
    CAN1->BTR = 0x001a0005; // Set Baud Rate to 500kbps
    // TODO: Loopback Mode For Testing
    CAN1->BTR |= CAN_BTR_LBKM; // Loopback Mode
    CAN1->MCR &= ~CAN_MCR_INRQ; // Exit Initialization Mode
    while (CAN1->MSR & CAN_MSR_INAK); // Wait until Normal Mode is entered
    CAN1_State = CAN_State_Ready;
}

/*
    TODO:   Seperate out the CAN init and CAN start
            Add CAN State Machine
*/

/**
 * @brief Transmit a CAN Frame
 * 
 * @param CAN [CAN_TypeDef*] CAN Peripheral to receive from
 * @param frame [CAN_Frame*] Frame to transmit
 * @return [CAN_Status] Status of Transmission
 */
CAN_Status CAN_Transmit(CAN_TypeDef* CAN, CAN_Frame* frame) {
    if (CAN == NULL || frame == NULL) {
        return CAN_Error;
    }

    uint8_t mailbox = Get_Empty_Mailbox();
    if (mailbox == 0xFFu) {
        return CAN_Mailbox_Error;
    }

    // Set ID, DLC, Frame Type, and Data
    CAN->sTxMailBox[mailbox].TIR = 0x0UL; // Clear the mailbox register
    CAN->sTxMailBox[mailbox].TIR |= (frame->id << CAN_TI0R_STID_Pos)
                                    | (frame->rtr << CAN_TI0R_RTR_Pos);
    CAN->sTxMailBox[mailbox].TDTR = (frame->dlc << CAN_TDT0R_DLC_Pos);
    CAN->sTxMailBox[mailbox].TDLR = (frame->data[0] << CAN_TDL0R_DATA0_Pos) 
                                    | (frame->data[1] << CAN_TDL0R_DATA1_Pos)
                                    | (frame->data[2] << CAN_TDL0R_DATA2_Pos) 
                                    | (frame->data[3] << CAN_TDL0R_DATA3_Pos);
    CAN->sTxMailBox[mailbox].TDHR = (frame->data[4] << CAN_TDH0R_DATA4_Pos) 
                                    | (frame->data[5] << CAN_TDH0R_DATA5_Pos) 
                                    | (frame->data[6] << CAN_TDH0R_DATA6_Pos) 
                                    | (frame->data[7] << CAN_TDH0R_DATA7_Pos);
    // Request Transmission
    CAN->sTxMailBox[mailbox].TIR |= CAN_TI0R_TXRQ;
    return CAN_TX_Req;
}

/**
 * @brief Receive a CAN Frame
 * @note Only populates id and data fields
 * 
 * @param CAN [CAN_TypeDef*] CAN Peripheral to receive from
 * @param frame [CAN_Frame*] Frame struct to receive
 * @return [CAN_Status] Status of Reception
 */
CAN_Status CAN_Receive(CAN_TypeDef* CAN, CAN_Frame* frame) {
    if (CAN == NULL || frame == NULL) {
        return CAN_Error;
    }

    if ((CAN->RF0R & CAN_RF0R_FMP0) == 0x1) {
        frame->id = (CAN->sFIFOMailBox[0].RIR & CAN_RI0R_STID_Msk) >> CAN_RI0R_STID_Pos;
        
        for (int i = 0; i < 4; i++) {
            frame->data[i] = (CAN->sFIFOMailBox[0].RDLR >> (i * 8)) & 0xFF;
        }
        for (int i = 0; i < 4; i++) {
            frame->data[i + 4] = (CAN->sFIFOMailBox[0].RDHR >> (i * 8)) & 0xFF;
        }
        CAN->RF0R |= CAN_RF0R_RFOM0; // Release FIFO 0
        return CAN_OK;
    }
}
    