/************************************************
* @file    can.c 
* @author  APBashara
* @date    7/2024
* 
* @brief   CAN Driver Implementation
***********************************************/

#include <stddef.h>

#include "stm32f415xx.h"
#include "can.h"

CAN_State CAN1_State;

/**
 * @brief Find an empty CAN Transmit mailbox
 * 
 * @return [uint8_t] Index of Empty Mailbox 
 */
static uint8_t Get_Empty_Mailbox() {
    return (CAN1->TSR & CAN_TSR_CODE_Msk) >> CAN_TSR_CODE_Pos;
}

CAN_Status CAN1_Init() {

    // Setup CAN Clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;

    // Setup CAN GPIO Pins for AF, Open Drain, and Very High Speed
    GPIOA->MODER &= ~GPIO_MODER_MODE11 & ~GPIO_MODER_MODE12;
    GPIOA->MODER |= (0x2 << GPIO_MODER_MODE11_Pos) | (0x2 << GPIO_MODER_MODE12_Pos);
    GPIOA->OTYPER |= GPIO_OTYPER_OT12;
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT11;
    GPIOA->PUPDR |= (0x1 << GPIO_PUPDR_PUPD12_Pos);
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD11;
    GPIOA->AFR[1] |= (0x9 << GPIO_AFRH_AFSEL11_Pos) | (0x9 << GPIO_AFRH_AFSEL12_Pos);
    GPIOA->OSPEEDR |= (0x2 << GPIO_OSPEEDR_OSPEED11_Pos) | (0x2 << GPIO_OSPEEDR_OSPEED12_Pos);

    // Initialize the CAN Peripheral
    CAN1->MCR |= CAN_MCR_RESET; // Reset CAN1
    CAN1->MCR &= ~CAN_MCR_SLEEP; // Exit Sleep Mode
    while (CAN1->MSR & CAN_MSR_SLAK); // Wait until Sleep Mode is exited
    CAN1->MCR |= CAN_MCR_INRQ; // Request Initialization Mode
    while (!(CAN1->MSR & CAN_MSR_INAK)); // Wait until Initialization Mode is entered
    CAN1_State = CAN_State_Initialization;

    // Configure CAN1 Settings
    CAN1->MCR &= ~CAN_MCR_TXFP & ~CAN_MCR_RFLM & ~CAN_MCR_TTCM 
                & ~CAN_MCR_ABOM & ~CAN_MCR_TXFP;
    CAN1->MCR |= CAN_MCR_AWUM | CAN_MCR_NART | CAN_MCR_DBF;
    
    // Configure CAN1 Baud Rate
    // http://www.bittiming.can-wiki.info/
    CAN1->BTR &= ~CAN_BTR_SILM & ~CAN_BTR_LBKM; // Disable Silent and Loopback Mode
    CAN1->BTR &= ~CAN_BTR_TS1_Msk & ~CAN_BTR_TS2_Msk 
                & ~CAN_BTR_SJW_Msk & ~CAN_BTR_BRP_Msk;
    CAN1->BTR |= (0xA << CAN_BTR_TS1_Pos) | (0x1 << CAN_BTR_TS2_Pos) 
                | (0x0 << CAN_BTR_SJW_Pos) | (0x5 << CAN_BTR_BRP_Pos);

    return CAN_OK;
}

CAN_Status CAN_Filters_Init() {
    if (CAN1_State != CAN_State_Initialization) {
        return CAN_Error;
    }

    CAN1->FMR |= CAN_FMR_FINIT; // Enter Filter Initialization Mode
    CAN1->FMR &= ~CAN_FMR_CAN2SB_Msk; 
    CAN1->FMR |= (28UL << CAN_FMR_CAN2SB_Pos); // Set Filter Start Bank to 0

    CAN1->FM1R |= CAN_FM1R_FBM_Msk; // Set Filters to ID mode
    CAN1->FS1R &= ~CAN_FS1R_FSC_Msk ; // Set Filters to Dual 16-bit mode

    // Create Filters for IDs and set masks to match IDs
    CAN1->sFilterRegister[0].FR1 = 0x048 << CAN_F0R1_FB5_Pos;
    CAN1->FFA1R &= ~CAN_FFA1R_FFA0; // Set Filter 0 to FIFO 0

    CAN1->sFilterRegister[0].FR1 |= 0x148 << CAN_F0R1_FB21_Pos;
    CAN1->FFA1R |= CAN_FFA1R_FFA1; // Set Filter 1 to FIFO 1

    CAN1->sFilterRegister[1].FR1 = 0x248 << CAN_F0R1_FB9_Pos;
    CAN1->FFA1R |= CAN_FFA1R_FFA2; // Set Filter 2 to FIFO 1

    CAN1->sFilterRegister[1].FR1 |= 0x548 << CAN_F0R1_FB21_Pos;
    CAN1->FFA1R |= CAN_FFA1R_FFA2; // Set Filter 2 to FIFO 1

    // Enable Filters
    CAN1->FA1R |= CAN_FA1R_FACT0 | CAN_FA1R_FACT1;
    CAN1->FMR &= ~CAN_FMR_FINIT; // Exit Filter Initialization Mode
    return CAN_OK;
}

CAN_Status CAN_Start() {
    if (CAN1_State != CAN_State_Initialization) {
        return CAN_Error;
    }
    CAN1->MCR &= ~CAN_MCR_INRQ; // Exit Initialization Mode
    while (CAN1->MSR & CAN_MSR_INAK); // Wait until Normal Mode is entered
    CAN1_State = CAN_State_Normal;

    return CAN_OK;
}

CAN_Status CAN_Stop() {
    if (CAN1_State != CAN_State_Normal) {
        return CAN_Error;
    }
    CAN1->MCR |= CAN_MCR_INRQ; // Enter Initialization Mode
    while (!(CAN1->MSR & CAN_MSR_INAK)); // Wait until Initialization Mode is entered
    CAN1_State = CAN_State_Initialization;
    return CAN_OK;
}

CAN_Status CAN_Transmit(CAN_TypeDef* CAN, CAN_Frame* frame) {
    // Basic Type Checking
    if (CAN == NULL || frame == NULL ||
        CAN1_State != CAN_State_Normal) {
        return CAN_Error;
    }

    volatile uint8_t mailbox = Get_Empty_Mailbox();

    // Clear the mailbox register
    CAN->sTxMailBox[mailbox].TIR = 0x0UL; 

    // Set ID, DLC, Frame Type, and Data
    CAN->sTxMailBox[mailbox].TIR |= (frame->id << CAN_TI0R_STID_Pos)
                                    | (frame->rtr << CAN_TI0R_RTR_Pos);
    CAN->sTxMailBox[mailbox].TDTR |= (frame->dlc << CAN_TDT0R_DLC_Pos);
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

CAN_Status CAN_Receive(CAN_TypeDef* CAN, CAN_Frame* frame) {
    if (CAN == NULL || frame == NULL ||
        CAN1_State != CAN_State_Normal) {
        return CAN_Error;
    }

    if ((CAN->RF0R & CAN_RF0R_FMP0)) {
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
    else if ((CAN->RF1R & CAN_RF1R_FMP1)) {
        frame->id = (CAN->sFIFOMailBox[1].RIR & CAN_RI1R_STID_Msk) >> CAN_RI1R_STID_Pos;

        for (int i = 0; i < 4; i++) {
            frame->data[i] = (CAN->sFIFOMailBox[1].RDLR >> (i * 8)) & 0xFF;
        }
        for (int i = 0; i < 4; i++) {
            frame->data[i + 4] = (CAN->sFIFOMailBox[1].RDHR >> (i * 8)) & 0xFF;
        }
        CAN->RF1R |= CAN_RF1R_RFOM1; // Release FIFO 1
        return CAN_OK;
    }
    else {
        return CAN_Fifo_Error;
    }
}

/* Interrupt Handlers -------------------------------------------------------*/
void CAN1_RX0_IRQHandler() {
    // Trigger task notification to handle RX packet
    BaseType_t xHPW;
    vTaskNotifyGiveFromISR(xCAN_Task, &xHPW);
    portYIELD_FROM_ISR(xHPW)
}