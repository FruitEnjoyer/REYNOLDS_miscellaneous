/**
 * @file can_protocol.c
 * @brief LPS22HB pressure sensor driver implementation
 * @date 28.01.2026
 * @author Ruslan Valeev
 * @details Implemented basic get-set communication functions
 */
#include "can_protocol.h"

#ifdef PUMPDRIVER


HAL_StatusTypeDef ConfigureFDCAN(FDCAN_HandleTypeDef* fdcan)
{
    HAL_StatusTypeDef res = HAL_OK;
    FDCAN_FilterTypeDef filterConfig = {
        .IdType = FDCAN_STANDARD_ID,
        .FilterIndex = 0,
        .FilterType = FDCAN_FILTER_MASK,
        .FilterConfig = FDCAN_FILTER_TO_RXFIFO0,
        .FilterID1 = 0x00000000,
        .FilterID2 = 0x00000000
    };
    res = HAL_FDCAN_ConfigFilter(fdcan, &filterConfig);
    if(res != HAL_OK) return res;
#if 0
    // TODO: возможно будет необходимо для включения фильтров
    res = HAL_FDCAN_ConfigGlobalFilter(fdcan,
                                    NonMatchingStd,
                                    NonMatchingExt,
                                    RejectRemoteStd,
                                    RejectRemoteExt);
#endif
    return res;
}

void ReadMessage(FDCAN_HandleTypeDef* fdcan, MessageData_PumpDriver_t* dest)
{
    FDCAN_RxHeaderTypeDef msgHeader;
    uint8_t data[8];

    HAL_FDCAN_GetRxMessage(fdcan, FDCAN_RX_FIFO0, &msgHeader, data);

    if(msgHeader.Identifier == COMMAND_PUMPDRIVER)
    {
        dest->topump.pwm_pump = (uint16_t)((data[0] << 8) + data[1]);
        dest->topump.pwm_heat = (uint16_t)((data[2] << 8) + data[3]);
    }
}

void SendMessage(FDCAN_HandleTypeDef* fdcan, Message_ID_t id, MessageData_PumpDriver_t* data)
{
    uint8_t buff[8] = {0};
    FDCAN_TxHeaderTypeDef msgHeader = {
        .Identifier = id,
        .IdType = FDCAN_STANDARD_ID,
        .TxFrameType = FDCAN_DATA_FRAME,
        .DataLength = FDCAN_DLC_BYTES_8,
        .ErrorStateIndicator = FDCAN_ESI_PASSIVE,
        .BitRateSwitch = FDCAN_BRS_OFF,
        .FDFormat = FDCAN_CLASSIC_CAN,
        .TxEventFifoControl = FDCAN_NO_TX_EVENTS,
        .MessageMarker = 0x45
    };

    buff[0] = (uint8_t)(data->tocpu.pump_speed >> 8);
    buff[1] = (uint8_t)(data->tocpu.pump_speed >> 0);
    buff[2] = (uint8_t)(data->tocpu.pump_current >> 8);
    buff[3] = (uint8_t)(data->tocpu.pump_current >> 0);

    if(HAL_FDCAN_GetTxFifoFreeLevel(fdcan) > 0)
    {
        HAL_FDCAN_AddMessageToTxFifoQ(fdcan, &msgHeader, buff);
    }
}
#endif

