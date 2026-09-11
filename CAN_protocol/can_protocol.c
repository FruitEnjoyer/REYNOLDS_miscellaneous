/**
 * @file can_protocol.c
 * @brief LPS22HB pressure sensor driver implementation
 * @date 28.01.2026
 * @author Ruslan Valeev
 * @details Implemented basic get-set communication functions
 */

#include "can_protocol.h"

#ifdef USE_CAN
HAL_StatusTypeDef ConfigureCAN(CAN_HandleTypeDef* can)
{
    HAL_StatusTypeDef res = HAL_OK;
    CAN_FilterTypeDef filterConfig = {
        .IdType = FDCAN_STANDARD_ID,
        .FilterIndex = 0,
        .FilterType = FDCAN_FILTER_MASK,
        .FilterConfig = FDCAN_FILTER_TO_RXFIFO0,
        .FilterID1 = 0x00000000,
        .FilterID2 = 0x00000000
    };
    res = HAL_CAN_ConfigFilter(can, &filterConfig);
    if(res != HAL_OK) return res;
    return res;
}

HAL_StatusTypeDef ReadMessage(CAN_HandleTypeDef* can, MessageData_t* dest)
{
    HAL_StatusTypeDef status = HAL_OK;
    CAN_RxHeaderTypeDef msgHeader;
    uint8_t data[8] = {0,};

    status = HAL_CAN_GetRxMessage(can, CAN_RX_FIFO0, &msgHeader, data);
    if(status == HAL_OK)
    {
        switch(msgHeader.StdId)
        {
        case DEFAULT_ID:
            // Impossible to receive message with default id
            break;
        case COMMAND_PUMPDRIVER:
            dest->pumpdriver.pwm_heat = (uint16_t)((data[0] << 8) + data[1]);
            dest->pumpdriver.pwm_pump = (uint16_t)((data[2] << 8) + data[3]);
            break;
        case RESPONSE_PUMPDRIVER:
            dest->pumpdriver.pump_current = (uint16_t)((data[0] << 8) + data[1]);
            dest->pumpdriver.pump_speed = (uint16_t)((data[2] << 8) + data[3]);
            break;
        default:
            // Received message with unknown id
            break;
        }
        dest->recv_id = msgHeader.StdId;
    }
    return status;
}

HAL_StatusTypeDef SendMessage(FDCAN_HandleTypeDef* fdcan, Message_ID_t id, MessageData_t* data)
{
    HAL_StatusTypeDef status = HAL_OK;
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
    switch(id)
    {
    case DEFAULT_ID:
        break;
    case COMMAND_PUMPDRIVER:
        buff[0] = (uint8_t)(data->pumpdriver.pwm_heat >> 8);
        buff[1] = (uint8_t)(data->pumpdriver.pwm_heat);
        buff[2] = (uint8_t)(data->pumpdriver.pwm_pump >> 8);
        buff[3] = (uint8_t)(data->pumpdriver.pwm_pump);
        break;
    case RESPONSE_PUMPDRIVER:
        buff[0] = (uint8_t)(data->pumpdriver.pump_current >> 8);
        buff[1] = (uint8_t)(data->pumpdriver.pump_current);
        buff[2] = (uint8_t)(data->pumpdriver.pump_speed >> 8);
        buff[3] = (uint8_t)(data->pumpdriver.pump_speed);
        break;
    default:
        break;
    }
    // Forbidden to send message with default id
    if(HAL_FDCAN_GetTxFifoFreeLevel(fdcan) > 0 && id != DEFAULT_ID)
    {
        status = HAL_FDCAN_AddMessageToTxFifoQ(fdcan, &msgHeader, buff);
    }
    else
    {  status = HAL_ERROR;  }
    return status;
}
#endif

#ifdef USE_FDCAN
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
    // TODO: it might be necessary to call this function
    res = HAL_FDCAN_ConfigGlobalFilter(fdcan,
                                    NonMatchingStd,
                                    NonMatchingExt,
                                    RejectRemoteStd,
                                    RejectRemoteExt);
#endif
    return res;
}

HAL_StatusTypeDef ReadMessage(FDCAN_HandleTypeDef* fdcan, MessageData_t* dest)
{
    HAL_StatusTypeDef status = HAL_OK;
    FDCAN_RxHeaderTypeDef msgHeader;
    uint8_t data[8] = {0,};

    status = HAL_FDCAN_GetRxMessage(fdcan, FDCAN_RX_FIFO0, &msgHeader, data);
    if(status == HAL_OK)
    {
        switch(msgHeader.Identifier)
        {
        case DEFAULT_ID:
            // Impossible to receive message with default id
            break;
        case COMMAND_PUMPDRIVER:
            dest->pumpdriver.pwm_heat = (uint16_t)((data[0] << 8) + data[1]);
            dest->pumpdriver.pwm_pump = (uint16_t)((data[2] << 8) + data[3]);
            break;
        case RESPONSE_PUMPDRIVER:
            dest->pumpdriver.pump_current = (uint16_t)((data[0] << 8) + data[1]);
            dest->pumpdriver.pump_speed = (uint16_t)((data[2] << 8) + data[3]);
            break;
        default:
            // Received message with unknown id
            break;
        }
        dest->recv_id = msgHeader.Identifier;
    }
    return status;
}

HAL_StatusTypeDef SendMessage(FDCAN_HandleTypeDef* fdcan, Message_ID_t id, MessageData_t* data)
{
    HAL_StatusTypeDef status = HAL_OK;
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
    switch(id)
    {
    case DEFAULT_ID:
        break;
    case COMMAND_PUMPDRIVER:
        buff[0] = (uint8_t)(data->pumpdriver.pwm_heat >> 8);
        buff[1] = (uint8_t)(data->pumpdriver.pwm_heat);
        buff[2] = (uint8_t)(data->pumpdriver.pwm_pump >> 8);
        buff[3] = (uint8_t)(data->pumpdriver.pwm_pump);
        break;
    case RESPONSE_PUMPDRIVER:
        buff[0] = (uint8_t)(data->pumpdriver.pump_current >> 8);
        buff[1] = (uint8_t)(data->pumpdriver.pump_current);
        buff[2] = (uint8_t)(data->pumpdriver.pump_speed >> 8);
        buff[3] = (uint8_t)(data->pumpdriver.pump_speed);
        break;
    default:
        break;
    }
    // Forbidden to send message with default id
    if(HAL_FDCAN_GetTxFifoFreeLevel(fdcan) > 0 && id != DEFAULT_ID)
    {
        status = HAL_FDCAN_AddMessageToTxFifoQ(fdcan, &msgHeader, buff);
    }
    else
    {  status = HAL_ERROR;  }
    return status;
}
#endif
