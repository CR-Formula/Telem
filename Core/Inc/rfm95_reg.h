/************************************************
* @file    rfm95_reg.h 
* @author  APBashara
* @date    6/2024
* 
* @brief   RFM95 Register Address Definitions
* @note    Addresses are for LoRa Mode
***********************************************/


/******************  Register Memory Address Macros  *******************/
#define RegFifo                                         (0x00u)
#define RegOpMode                                       (0x01u)
#define RegFrfMsb                                       (0x06u)
#define RegFrfMid                                       (0x07u)
#define RegFrfLsb                                       (0x08u)
#define RegPaConfig                                     (0x09u)
#define RegPaRamp                                       (0x0Au)
#define RegOcp                                          (0x0Bu)
#define RegLna                                          (0x0Cu)
#define RegFifoAddrPtr                                  (0x0Du)
#define RegFifoTxBaseAddr                               (0x0Eu)
#define RegFifoRxBaseAddr                               (0x0Fu)
#define RegFifoRxCurrentAddr                            (0x10u)
#define RegRxTimeoutMask                                (0x11u)
#define RegIrqFlags                                     (0x12u)
#define RegRxNbBytes                                    (0x13u)
#define RegRxHeaderCntValueMsb                          (0x14u)
#define RegRxHeaderCntValueLsb                          (0x15u)
#define RegRxPacketCntValueMsb                          (0x16u)
#define RegRxPacketCntValueLsb                          (0x17u)
#define RegModemStat                                    (0x18u)
#define RegPktSnrValue                                  (0x19u)
#define RegPktRssiValue                                 (0x1Au)
#define RegRssiValue                                    (0x1Bu)
#define RegHopChannel                                   (0x1Cu)
#define RegModemConfig1                                 (0x1Du)
#define RegModemConfig2                                 (0x1Eu)
#define RegSymbTimeoutLsb                               (0x1Fu)
#define RegPreambleMsb                                  (0x20u)
#define RegPreambleLsb                                  (0x21u)
#define RegPayloadLength                                (0x22u)
#define RegMaxPayloadLength                             (0x23u)
#define RegHopPeriod                                    (0x24u)
#define RegFifoRxByteAddr                               (0x25u)
#define RegModemConfig3                                 (0x26u)
#define RegPpmCorrection                                (0x27u)
#define RegFeiMsb                                       (0x28u)
#define RegFeiMid                                       (0x29u)
#define RegFeiLsb                                       (0x2Au)
#define RegRssiWideband                                 (0x2Cu)
#define RegDetectOptimize                               (0x31u)
#define RegInvertIQ                                     (0x33u)
#define RegDetectionThreshold                           (0x37u)
#define RegSyncWord                                     (0x39u)

#define RegDioMapping1                                  (0x40u)
#define RegDioMapping2                                  (0x41u)
#define RegVersion                                      (0x42u)
#define RegTcxo                                         (0x4Bu)
#define RegPaDac                                        (0x4Du)
#define RegFormerTemp                                   (0x5Bu)
#define RegAgcRef                                       (0x61u)
#define RegAgcThresh1                                   (0x62u)
#define RegAgcThresh2                                   (0x63u)
#define RegAgcThresh3                                   (0x64u)

/***********************  RegFifo  ************************/
#define RegFifo_Fifo_Pos                                (0u)
#define RegFifo_Fifo_Msk                                (0xFFu << RegFifo_Fifo_Pos)
#define RegFifo_Fifo                                    RegFifo_Fifo_Msk

/**********************  RegOpMode  ***********************/
#define RegOpMode_Mode_Pos                              (0u)
#define RegOpMode_Mode_Msk                              (0x3u << RegOpMode_Mode_Pos)
#define RegOpMode_Mode                                  RegOpMode_Mode_Msk
#define RegOpMode_Mode_0                                (0x1u << RegOpMode_Mode_Pos)
#define RegOpMode_Mode_1                                (0x2u << RegOpMode_Mode_Pos)
#define RegOpMode_Mode_2                                (0x4u << RegOpMode_Mode_Pos)
#define RegOpMode_LowFrequencyModeOn_Pos                (3u)
#define RegOpMode_LowFrequencyModeOn_Msk                (0x1 << RegOpMode_LowFrequencyModeOn_Pos)
#define RegOpMode_LowFrequencyModeOn                    RegOpMode_LowFrequencyModeOn_Msk
#define RegOpMode_AccessSharedReg_Pos                   (6u)
#define RegOpMode_AccessSharedReg_Msk                   (0x1 << RegOpMode_AccessSharedReg_Pos)
#define RegOpMode_AccessSharedReg                       RegOpMode_AccessSharedReg_Msk
#define RegOpMode_LongRangeMode_Pos                     (7u)
#define RegOpMode_LongRangeMode_Msk                     (0x1u << RegOpMode_LongRangeMode_Pos)
#define RegOpMode_LongRangeMode                         RegOpMode_LongRangeMode_Msk

/***********************  RegFrMsb  ************************/
#define RegFrMsb_Frf_Pos                                (0u)
#define RegFrMsb_Frf_Msk                                (0xFFu << RegFrMsb_Frf_Pos)
#define RegFrMsb_Frf                                    RegFrMsb_Frf_Msk

/***********************  RegFrMid  ************************/
#define RegFrMid_Frf_Pos                                (0u)
#define RegFrMid_Frf_Msk                                (0xFFu << RegFrMid_Frf_Pos)
#define RegFrMid_Frf                                    RegFrMid_Frf_Msk

/***********************  RegFrLsb  ************************/
#define RegFrLsb_Frf_Pos                                (0u)
#define RegFrLsb_Frf_Msk                                (0xFFu << RegFrLsb_Frf_Pos)
#define RegFrLsb_Frf                                    RegFrLsb_Frf_Msk

/**********************  RegPaConfig  **********************/
#define RegPaConfig_OutputPower_Pos                     (0u)
#define RegPaConfig_OutputPower_Msk                     (0xFu << RegPaConfig_OutputPower_Pos)
#define RegPaConfig_OutputPower                         RegPaConfig_OutputPower_Msk
#define RegPaConfig_MaxPower_Pos                        (4u)
#define RegPaConfig_MaxPower_Msk                        (0x7u << RegPaConfig_MaxPower_Pos)
#define RegPaConfig_MaxPower                            RegPaConfig_MaxPower_Msk
#define RegPaConfig_PaSelect_Pos                        (7u)
#define RegPaConfig_PaSelect_Msk                        (0x1u << RegPaConfig_PaSelect_Pos)
#define RegPaConfig_PaSelect                            RegPaConfig_PaSelect_Msk

#define RegPaConfig_20dBm                               0xFF

/***********************  RegPaRamp  ***********************/
#define RegPaRamp_PaRamp_Pos                            (0u)
#define RegPaRamp_PaRamp_Msk                            (0xFu << RegPaRamp_PaRamp_Pos)
#define RegPaRamp_PaRamp                                RegPaRamp_PaRamp_Msk

/***********************  RegOcp  *************************/
#define RegOcp_OcpTrim_Pos                              (0u)
#define RegOcp_OcpTrim_Msk                              (0x1Fu << RegOcp_OcpTrim_Pos)
#define RegOcp_OcpTrim                                  RegOcp_OcpTrim_Msk
#define RegOcp_OcpOn_Pos                                (5u)
#define RegOcp_OcpOn_Msk                                (0x1u << RegOcp_OcpOn_Pos)
#define RegOcp_OcpOn                                    RegOcp_OcpOn_Msk

/***********************  RegLna  *************************/
#define RegLnaBoostHf_Pos                               (0u)
#define RegLnaBoostHf_Msk                               (0x3u << RegLnaBoostHf_Pos)
#define RegLnaBoostHf                                   RegLnaBoostHf_Msk
#define RegLnaBoostLf_Pos                               (3u)
#define RegLnaBoostLf_Msk                               (0x3u << RegLnaBoostLf_Pos)
#define RegLnaBoostLf                                   RegLnaBoostLf_Msk
#define RegLnaGain_Pos                                  (5u)
#define RegLnaGain_Msk                                  (0x7u << RegLnaGain_Pos)
#define RegLnaGain                                      RegLnaGain_Msk

/*******************  RegFifoAddrPtr  *********************/
#define RegFifoAddrPtr_FifoAddrPtr_Pos                  (0u)
#define RegFifoAddrPtr_FifoAddrPtr_Msk                  (0xFFu << RegFifoAddrPtr_FifoAddrPtr_Pos)
#define RegFifoAddrPtr_FifoAddrPtr                      RegFifoAddrPtr_FifoAddrPtr_Msk

/*******************  RegFifoTxBaseAddr  ******************/
#define RegFifoTxBaseAddr_FifoTxBaseAddr_Pos            (0u)
#define RegFifoTxBaseAddr_FifoTxBaseAddr_Msk            (0xFFu << RegFifoTxBaseAddr_FifoTxBaseAddr_Pos)
#define RegFifoTxBaseAddr_FifoTxBaseAddr                RegFifoTxBaseAddr_FifoTxBaseAddr_Msk

/*******************  RegFifoRxBaseAddr  ******************/
#define RegFifoRxBaseAddr_FifoRxBaseAddr_Pos            (0u)
#define RegFifoRxBaseAddr_FifoRxBaseAddr_Msk            (0xFFu << RegFifoRxBaseAddr_FifoRxBaseAddr_Pos)
#define RegFifoRxBaseAddr_FifoRxBaseAddr                RegFifoRxBaseAddr_FifoRxBaseAddr_Msk

/*********************  RegFifoRxCurrentAddress  **********************/
#define RegFifoRxCurrentAddr_FifoRxCurrentAddr_Pos      (0u)
#define RegFifoRxCurrentAddr_FifoRxCurrentAddr_Msk      (0xFFu << RegFifoRxCurrentAddr_FifoRxCurrentAddr_Pos)
#define RegFifoRxCurrentAddr_FifoRxCurrentAddr          RegFifoRxCurrentAddr_FifoRxCurrentAddr_Msk

/*********************  RegIrqFlagsMask  **********************/
#define RegIrqFlags_CadDetectedMask_Pos                 (0u)
#define RegIrqFlags_CadDetectedMask_Msk                 (0x1u << RegIrqFlags_CadDetectedMask_Pos)
#define RegIrqFlags_CadDetectedMask                     RegIrqFlags_CadDetectedMask_Msk
#define RegIrqFlags_FhssChangeChannelMask_Pos           (1u)
#define RegIrqFlags_FhssChangeChannelMask_Msk           (0x1u << RegIrqFlags_FhssChangeChannelMask_Pos)
#define RegIrqFlags_FhssChangeChannelMask               RegIrqFlags_FhssChangeChannelMask_Msk
#define RegIrqFlags_CadDoneMask_Pos                     (2u)
#define RegIrqFlags_CadDoneMask_Msk                     (0x1u << RegIrqFlags_CadDoneMask_Pos)
#define RegIrqFlags_CadDoneMask                         RegIrqFlags_CadDoneMask_Msk
#define RegIrqFlags_TxDoneMask_Pos                      (3u)
#define RegIrqFlags_TxDoneMask_Msk                      (0x1u << RegIrqFlags_TxDoneMask_Pos)
#define RegIrqFlags_TxDoneMask                          RegIrqFlags_TxDoneMask_Msk
#define RegIrqFlags_ValidHeaderMask_Pos                 (4u)
#define RegIrqFlags_ValidHeaderMask_Msk                 (0x1u << RegIrqFlags_ValidHeaderMask_Pos)
#define RegIrqFlags_ValidHeaderMask                     RegIrqFlags_ValidHeaderMask_Msk
#define RegIrqFlags_PayloadCrcErrorMask_Pos             (5u)
#define RegIrqFlags_PayloadCrcErrorMask_Msk             (0x1u << RegIrqFlags_PayloadCrcErrorMask_Pos)
#define RegIrqFlags_PayloadCrcErrorMask                 RegIrqFlags_PayloadCrcErrorMask_Msk
#define RegIrqFlags_RxDoneMask_Pos                      (6u)
#define RegIrqFlags_RxDoneMask_Msk                      (0x1u << RegIrqFlags_RxDoneMask_Pos)
#define RegIrqFlags_RxDoneMask                          RegIrqFlags_RxDoneMask_Msk
#define RegIrqFlags_RxTimeoutMask_Pos                   (7u)
#define RegIrqFlags_RxTimeoutMask_Msk                   (0x1u << RegIrqFlags_RxTimeoutMask_Pos)
#define RegIrqFlags_RxTimeoutMask                       RegIrqFlags_RxTimeoutMask_Msk

/*********************  RegIrqFlags  **********************/
#define RegIrqFlags_CadDetected_Pos                     (0u)
#define RegIrqFlags_CadDetected_Msk                     (0x1u << RegIrqFlags_CadDetected_Pos)
#define RegIrqFlags_CadDetected                         RegIrqFlags_CadDetected_Msk
#define RegIrqFlags_FhssChangeChannel_Pos               (1u)
#define RegIrqFlags_FhssChangeChannel_Msk               (0x1u << RegIrqFlags_FhssChangeChannel_Pos)
#define RegIrqFlags_FhssChangeChannel                   RegIrqFlags_FhssChangeChannel_Msk
#define RegIrqFlags_CadDone_Pos                         (2u)
#define RegIrqFlags_CadDone_Msk                         (0x1u << RegIrqFlags_CadDone_Pos)
#define RegIrqFlags_CadDone                             RegIrqFlags_CadDone_Msk
#define RegIrqFlags_TxDone_Pos                          (3u)
#define RegIrqFlags_TxDone_Msk                          (0x1u << RegIrqFlags_TxDone_Pos)
#define RegIrqFlags_TxDone                              RegIrqFlags_TxDone_Msk
#define RegIrqFlags_ValidHeader_Pos                     (4u)
#define RegIrqFlags_ValidHeader_Msk                     (0x1u << RegIrqFlags_ValidHeader_Pos)
#define RegIrqFlags_ValidHeader                         RegIrqFlags_ValidHeader_Msk
#define RegIrqFlags_PayloadCrcError_Pos                 (5u)
#define RegIrqFlags_PayloadCrcError_Msk                 (0x1u << RegIrqFlags_PayloadCrcError_Pos)
#define RegIrqFlags_PayloadCrcError                     RegIrqFlags_PayloadCrcError_Msk
#define RegIrqFlags_RxDone_Pos                          (6u)
#define RegIrqFlags_RxDone_Msk                          (0x1u << RegIrqFlags_RxDone_Pos)
#define RegIrqFlags_RxDone                              RegIrqFlags_RxDone_Msk
#define RegIrqFlags_RxTimeout_Pos                       (7u)
#define RegIrqFlags_RxTimeout_Msk                       (0x1u << RegIrqFlags_RxTimeout_Pos)
#define RegIrqFlags_RxTimeout                           RegIrqFlags_RxTimeout_Msk

/*********************  RegRxNbBytes  **********************/
#define RegRxNbBytes_RxNbBytes_Pos                      (0u)
#define RegRxNbBytes_RxNbBytes_Msk                      (0xFFu << RegRxNbBytes_RxNbBytes_Pos)
#define RegRxNbBytes_RxNbBytes                          RegRxNbBytes_RxNbBytes_Msk

/*********************  RegRxHeaderCntValueMsb  **********************/
#define RegRxHeaderCntValueMsb_ValidHeaderCntMsb_Pos    (0u)
#define RegRxHeaderCntValueMsb_ValidHeaderCntMsb_Msk    (0xFFu << RegRxHeaderCntValueMsb_ValidHeaderCntMsb_Pos)
#define RegRxHeaderCntValueMsb_ValidHeaderCntMsb        RegRxHeaderCntValueMsb_ValidHeaderCntMsb_Msk

/*********************  RegRxHeaderCntValueLsb  **********************/
#define RegRxHeaderCntValueLsb_ValidHeaderCntLsb_Pos    (0u)
#define RegRxHeaderCntValueLsb_ValidHeaderCntLsb_Msk    (0xFFu << RegRxHeaderCntValueLsb_ValidHeaderCntLsb_Pos)
#define RegRxHeaderCntValueLsb_ValidHeaderCntLsb        RegRxHeaderCntValueLsb_ValidHeaderCntLsb_Msk

/*********************  RegRxPacketCntValueMsb   **********************/
#define RegRxPacketCntValueMsb_RxPacketCntValueMsb_Pos  (0u)
#define RegRxPacketCntValueMsb_RxPacketCntValueMsb_Msk  (0xFFu << RegRxPacketCntValueMsb_RxPacketCntValueMsb_Pos)
#define RegRxPacketCntValueMsb_RxPacketCntValueMsb      RegRxPacketCntValueMsb_RxPacketCntValueMsb_Msk

/*********************  RegRxPacketCntValueLsb   **********************/
#define RegRxPacketCntValueLsb_RxPacketCntValueLsb_Pos  (0u)
#define RegRxPacketCntValueLsb_RxPacketCntValueLsb_Msk  (0xFFu << RegRxPacketCntValueLsb_RxPacketCntValueLsb_Pos)
#define RegRxPacketCntValueLsb_RxPacketCntValueLsb      RegRxPacketCntValueLsb_RxPacketCntValueLsb_Msk

/*********************  RegModemStat  **********************/
#define RegModemStat_Signal_Detect_Pos                  (0u)
#define RegModemStat_Signal_Detect_Msk                  (0x1u << RegModemStat_Signal_Detect_Pos)
#define RegModemStat_Signal_Detect                      RegModemStat_Signal_Detect_Msk
#define RegModemStat_Signal_Sync_Pos                    (1u)
#define RegModemStat_Signal_Sync_Msk                    (0x1u << RegModemStat_Signal_Sync_Pos)
#define RegModemStat_Signal_Sync                        RegModemStat_Signal_Sync_Msk
#define RegModemStat_Rx_Ongoing_Pos                     (2u)
#define RegModemStat_Rx_Ongoing_Msk                     (0x1u << RegModemStat_Rx_Ongoing_Pos)
#define RegModemStat_Rx_Ongoing                         RegModemStat_Rx_Ongoing_Msk
#define RegModemStat_Header_Info_Valid_Pos              (3u)
#define RegModemStat_Header_Info_Valid_Msk              (0x1u << RegModemStat_Header_Info_Valid_Pos)
#define RegModemStat_Header_Info_Valid                  RegModemStat_Header_Info_Valid_Msk
#define RegModemStat_Modem_Clear_Pos                    (4u)
#define RegModemStat_Modem_Clear_Msk                    (0x1u << RegModemStat_Modem_Clear_Pos)
#define RegModemStat_Modem_Clear                        RegModemStat_Modem_Clear_Msk
#define RegModemStat_RxCodingRate_Pos                   (5u)
#define RegModemStat_RxCodingRate_Msk                   (0x7u << RegModemStat_RxCodingRate_Pos)

/*********************  RegPktSnrValue  **********************/
#define RegPktSnrValue_PktSnrValue_Pos                  (0u)
#define RegPktSnrValue_PktSnrValue_Msk                  (0xFFu << RegPktSnrValue_PktSnrValue_Pos)
#define RegPktSnrValue_PktSnrValue                      RegPktSnrValue_PktSnrValue_Msk

/*********************  RegPktRssiValue  **********************/
#define RegPktRssiValue_PacketRssi_Pos                  (0u)
#define RegPktRssiValue_PacketRssi_Msk                  (0xFFu << RegPktRssiValue_PacketRssi_Pos)
#define RegPktRssiValue_PacketRssi                      RegPktRssiValue_PacketRssi_Msk

/*********************  RegRssiValue  **********************/
#define RegRssiValue_Rssi_Pos                           (0u)
#define RegRssiValue_Rssi_Msk                           (0xFFu << RegRssiValue_Rssi_Pos)
#define RegRssiValue_Rssi                               RegRssiValue_Rssi_Msk

/*********************  RegHopChannel  **********************/
#define RegHopChannel_FhssPresentChannel_Pos            (0u)
#define RegHopChannel_FhssPresentChannel_Msk            (0x3Fu << RegHopChannel_FhssPresentChannel_Pos)
#define RegHopChannel_FhssPresentChannel                RegHopChannel_FhssPresentChannel_Msk
#define RegHopChannel_RxPayloadCrcOn_Pos                (6u)
#define RegHopChannel_RxPayloadCrcOn_Msk                (0x1u << RegHopChannel_RxPayloadCrcOn_Pos)
#define RegHopChannel_RxPayloadCrcOn                    RegHopChannel_RxPayloadCrcOn_Msk
#define RegHopChannel_PllTimeout_Pos                    (7u)
#define RegHopChannel_PllTimeout_Msk                    (0x1u << RegHopChannel_PllTimeout_Pos)
#define RegHopChannel_PllTimeout                        RegHopChannel_PllTimeout_Msk

/*********************  RegModemConfig1  **********************/
#define RegModemConfig1_ImplicitHeaderModeOn_Pos        (0u)
#define RegModemConfig1_ImplicitHeaderModeOn_Msk        (0x1u << RegModemConfig1_ImplicitHeaderModeOn_Pos)
#define RegModemConfig1_ImplicitHeaderModeOn            RegModemConfig1_ImplicitHeaderModeOn_Msk
#define RegModemConfig1_CodingRate_Pos                  (1u)
#define RegModemConfig1_CodingRate_Msk                  (0x7u << RegModemConfig1_CodingRate_Pos)
#define RegModemConfig1_CodingRate                      RegModemConfig1_CodingRate_Msk
#define RegModemConfig1_Bw_Pos                          (4u)
#define RegModemConfig1_Bw_Msk                          (0xFu << RegModemConfig1_Bw_Pos)
#define RegModemConfig1_Bw                              RegModemConfig1_Bw_Msk

/*********************  RegModemConfig2  **********************/
#define RegModemConfig2_SymbTimeoutMsb_Pos              (0u)
#define RegModemConfig2_SymbTimeoutMsb_Msk              (0x3u << RegModemConfig2_SymbTimeoutMsb_Pos)
#define RegModemConfig2_SymbTimeoutMsb                  RegModemConfig2_SymbTimeoutMsb_Msk
#define RegModemConfig2_RxPayloadCrcOn_Pos              (2u)
#define RegModemConfig2_RxPayloadCrcOn_Msk              (0x1u << RegModemConfig2_RxPayloadCrcOn_Pos)
#define RegModemConfig2_RxPayloadCrcOn                  RegModemConfig2_RxPayloadCrcOn_Msk
#define RegModemConfig2_TxContinuousMode_Pos            (3u)
#define RegModemConfig2_TxContinuousMode_Msk            (0x1u << RegModemConfig2_TxContinuousMode_Pos)
#define RegModemConfig2_TxContinuousMode                RegModemConfig2_TxContinuousMode_Msk
#define RegModemConfig2_SpreadingFactor_Pos             (4u)
#define RegModemConfig2_SpreadingFactor_Msk             (0xFu << RegModemConfig2_SpreadingFactor_Pos)
#define RegModemConfig2_SpreadingFactor                 RegModemConfig2_SpreadingFactor_Msk

/*********************  RegSymbTimeoutLsb  **********************/
#define RegSymbTimeoutLsb_SymbTimeoutLsb_Pos            (0u)
#define RegSymbTimeoutLsb_SymbTimeoutLsb_Msk            (0xFFu << RegSymbTimeoutLsb_SymbTimeoutLsb_Pos)
#define RegSymbTimeoutLsb_SymbTimeoutLsb                RegSymbTimeoutLsb_SymbTimeoutLsb_Msk

/*********************  RegPreambleMsb  **********************/
#define RegPreambleMsb_PreambleMsb_Pos                  (0u)
#define RegPreambleMsb_PreambleMsb_Msk                  (0xFFu << RegPreambleMsb_PreambleMsb_Pos)
#define RegPreambleMsb_PreambleMsb                      RegPreambleMsb_PreambleMsb_Msk

/*********************  RegPreambleLsb  **********************/
#define RegPreambleLsb_PreambleLsb_Pos                  (0u)
#define RegPreambleLsb_PreambleLsb_Msk                  (0xFFu << RegPreambleLsb_PreambleLsb_Pos)
#define RegPreambleLsb_PreambleLsb                      RegPreambleLsb_PreambleLsb_Msk

/*********************  RegPayloadLength  **********************/
#define RegPayloadLength_PayloadLength_Pos              (0u)
#define RegPayloadLength_PayloadLength_Msk              (0xFFu << RegPayloadLength_PayloadLength_Pos)
#define RegPayloadLength_PayloadLength                  RegPayloadLength_PayloadLength_Msk

/*********************  RegMaxPayloadLength   **********************/
#define RegMaxPayloadLength_MaxPayloadLength_Pos        (0u)
#define RegMaxPayloadLength_MaxPayloadLength_Msk        (0xFFu << RegMaxPayloadLength_MaxPayloadLength_Pos)
#define RegMaxPayloadLength_MaxPayloadLength            RegMaxPayloadLength_MaxPayloadLength_Msk

/*********************  RegHopPeriod  **********************/
#define RegHopPeriod_FreqHoppingPeriod_Pos              (0u)
#define RegHopPeriod_FreqHoppingPeriod_Msk              (0xFFu << RegHopPeriod_FreqHoppingPeriod_Pos)
#define RegHopPeriod_FreqHoppingPeriod                  RegHopPeriod_FreqHoppingPeriod_Msk

/*********************  RegFifoRxByteAddr  **********************/
#define RegFifoRxByteAddr_FifoRxByteAddrPtr_Pos         (0u)
#define RegFifoRxByteAddr_FifoRxByteAddrPtr_Msk         (0xFFu << RegFifoRxByteAddr_FifoRxByteAddrPtr_Pos)
#define RegFifoRxByteAddr_FifoRxByteAddrPtr             RegFifoRxByteAddr_FifoRxByteAddrPtr_Msk

/*********************  RegModemConfig3  **********************/
#define RegModemConfig3_AgcAutoOn_Pos                   (2u)
#define RegModemConfig3_AgcAutoOn_Msk                   (0x1u << RegModemConfig3_AgcAutoOn_Pos)
#define RegModemConfig3_AgcAutoOn                       RegModemConfig3_AgcAutoOn_Msk
#define RegModemConfig3_LowDataRateOpt_Pos              (3u)
#define RegModemConfig3_LowDataRateOpt_Msk              (0x1u << RegModemConfig3_LowDataRateOpt_Pos)
#define RegModemConfig3_LowDataRateOpt                  RegModemConfig3_LowDataRateOpt_Msk

/*********************  RegPaDac  **********************/
#define RegPaDac_PaDac_Pos                              (0u)
#define RegPaDac_PaDac_Msk                              (0x7u << RegPaDac_PaDac_Pos)
#define RegPaDac_PaDac                                  RegPaDac_PaDac_Msk

#define RegPaDac_20dBm                                  0x87
