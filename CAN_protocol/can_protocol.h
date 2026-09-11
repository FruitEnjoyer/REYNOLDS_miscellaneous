/**
 * @file can_protocol.h
 * @author ruslan
 * @brief Header for CAN communication interface
 * @date 28.01.2026
 */

#ifndef CAN_PROTOCOL_H_
#define CAN_PROTOCOL_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "main.h"
#include <stdint.h>

#if !defined(PUMPDRIVER) && !defined(MAINCONTROLLER)
  #error "Please define board!"
#endif

#define USE_CAN
//#define USE_FDCAN
#if defined(USE_CAN) && defined(USE_FDCAN)
#error "Use either CAN or FDCAN only"
#endif

// Define STB port and pin in CubeMX to enable TJA1042

#if !defined(TJA1042_STB_GPIO_Port) || !defined(TJA1042_STB_Pin)
  #error "Please define STB port and pin"
#endif


/**
 * @brief CAN message identificator
 */
typedef enum Message_ID
{
    DEFAULT_ID = 0xFFFF,

    // commands from main controller
    COMMAND_PUMPDRIVER = 0x0100,

    // responses to main controller
    RESPONSE_PUMPDRIVER = 0x0200
} Message_ID_t;

/**
 * @brief Data structure for communication via CAN
 *
 * @details Each substructure contains data for dedicated device
 *          to be transceived
 */
typedef struct MessageData
{
    // Last received message ID
    Message_ID_t recv_id;
    
    struct{
        uint16_t pwm_pump, pwm_heat;
        uint16_t pump_speed, pump_current;
    } pumpdriver;
} MessageData_t;

#ifdef USE_CAN
/**
 * @brief Configure CAN peripheral with desired settings
 *
 * @param can pointer to an CAN handle structure
 * @return HAL_StatusTypeDef result of configurating
 */
HAL_StatusTypeDef ConfigureCAN(CAN_HandleTypeDef* can);

/**
 * @brief Read CAN Rx buffer & interpret its data
 *
 * @param can pointer to an CAN handle structure
 * @param dest pointer to a destination data structure
 * @return HAL_StatusTypeDef result of reading
 */
HAL_StatusTypeDef ReadMessage(CAN_HandleTypeDef* can, MessageData_t* dest);

/**
 * @brief Send message with dedicated identifier into the bus
 *
 * @param can pointer to an CAN handle structure
 * @param id CAN message identificator
 * @param data pointer to source data structure
 * @return HAL_StatusTypeDef result of sending
 */
HAL_StatusTypeDef SendMessage(CAN_HandleTypeDef* can, Message_ID_t id, MessageData_t* data);
#endif

#ifdef USE_FDCAN
/**
 * @brief Configure FDCAN peripheral with desired settings
 * 
 * @param fdcan pointer to an FDCAN handle structure
 * @return HAL_StatusTypeDef result of configurating
 */
HAL_StatusTypeDef ConfigureFDCAN(FDCAN_HandleTypeDef* fdcan);

/**
 * @brief Read FDCAN Rx buffer & interpret its data
 * 
 * @param fdcan pointer to an FDCAN handle structure
 * @param dest pointer to a destination data structure
 * @return HAL_StatusTypeDef result of reading
 */
HAL_StatusTypeDef ReadMessage(FDCAN_HandleTypeDef* fdcan, MessageData_t* dest);

/**
 * @brief Send message with dedicated identifier into the bus
 * 
 * @param fdcan pointer to an FDCAN handle structure
 * @param id CAN message identificator
 * @param data pointer to source data structure
 * @return HAL_StatusTypeDef result of sending
 */
HAL_StatusTypeDef SendMessage(FDCAN_HandleTypeDef* fdcan, Message_ID_t id, MessageData_t* data);
#endif

#ifdef __cplusplus
}
#endif

#endif /* CAN_PROTOCOL_H_ */
