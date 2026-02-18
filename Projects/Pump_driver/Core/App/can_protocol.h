/**
 * @file can_protocol.h
 * @brief Header for CAN communication interface
 * @date 28.01.2026
 * @author Ruslan Valeev
 */

#ifndef APP_CAN_PROTOCOL_H_
#define APP_CAN_PROTOCOL_H_

#include "main.h"

// temporary defines
// in future use -DDEFINE1 -DDEFINE2
//#define MAINCONTROLLER
#define PUMPDRIVER

typedef enum Message_ID
{
    // commands from main controller
    COMMAND_PUMPDRIVER = 0x0100,

    // responses to main controller
    RESPONSE_PUMPDRIVER = 0x0200
} Message_ID_t;

typedef struct MessageData_PumpDriver
{
    struct{
        uint16_t pwm_pump, pwm_heat;
    } topump;
    struct{
        uint16_t pump_speed, pump_current;
    } tocpu;
} MessageData_PumpDriver_t;


#ifdef PUMPDRIVER
HAL_StatusTypeDef ConfigureFDCAN(FDCAN_HandleTypeDef* fdcan);
void ReadMessage(FDCAN_HandleTypeDef* fdcan, MessageData_PumpDriver_t* dest);
void SendMessage(FDCAN_HandleTypeDef* fdcan, Message_ID_t id, MessageData_PumpDriver_t* data);
#endif

#endif /* APP_CAN_PROTOCOL_H_ */
