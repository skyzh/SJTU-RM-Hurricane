//
// Created by Alex Chi on 2018/11/12.
//

#include "hal.h"
#include "OI.h"
#include "HurricaneArmSystem.h"
#include "HurricaneDebugSystem.h"
#include "HurricaneCANSystem.h"
#include "CAHRR/src/utils.h"
#include "Models/Arm.h"
#include "Models/ArmTop.h"
#include "Models/ArmBottom.h"
#include "Models/ArmBase.h"

#include <cmath>

#define CLAW_ADC hadc1

// #define HURRICANE_ARM_DISABLE

const int M2006_ANGLE_ID = 0;
const int M2006_SPEED_ID = 1;
const int M3508_ANGLE_ID = 0;
const int M3508_SPEED_ID = 1;

const double PID_SPEED_LIMIT = 1000;
const double PID_CURRENT_LIMIT = 2000;

HurricaneArmSystem::HurricaneArmSystem() : position(0), data_available_bottom(false), data_available_base(false) {
    // Bottom Arm
    this->arm_bottom = new ArmBottom(
            // Speed P I D and Limit
            0.5, 0.0, 1.0, PID_CURRENT_LIMIT,
            // Position P I D and Limit
            30.0, 0.0, 10.0, PID_SPEED_LIMIT,
            // Ramp, Feed Forward
            800, 0);
    this->arm_top = new ArmTop(
            // Speed P I D and Limit
            0.0, 0.0, 1.0, PID_CURRENT_LIMIT,
            // Position P I D and Limit
            0.0, 0.0, 0.0, PID_SPEED_LIMIT,
            // Ramp, Feed Forward
            800, 0);
    this->arm_base = new ArmBase(
            // Speed P I D and Limit
            0.5, 0.0, 1.0, PID_CURRENT_LIMIT,
            // Position P I D and Limit
            30.0, 0.0, 10.0, PID_SPEED_LIMIT,
            // Ramp, Feed Forward
            800, 0);
    this->arm_bottom->top = this->arm_top;
    this->arm_top->bottom = this->arm_bottom;
}

bool HurricaneArmSystem::initialize() {
    OK(oi->debugSystem->info("ARM", "arm system initialize"));
    HAL_ADC_Start_IT(&CLAW_ADC);
    OK(this->arm_bottom->initialize());
    OK(this->arm_top->initialize());
    OK(this->arm_base->initialize());
    OK(oi->CANSystem->set(ARM_BOTTOM_ID, 0));
    OK(oi->CANSystem->set(ARM_BASE_ID, 0));
    OK(oi->debugSystem->info("ARM", "  ... complete"));
    return true;
}

bool HurricaneArmSystem::update() {
    OK(this->arm_bottom->update());
    OK(this->arm_base->update());
    // todo: arm_top update
    if (!disabled) {
        OK(oi->CANSystem->set(ARM_BOTTOM_ID, this->arm_bottom->output()));
        OK(oi->CANSystem->set(ARM_BASE_ID, this->arm_base->output()));
    } else {
        OK(oi->CANSystem->set(ARM_BOTTOM_ID, 0));
        OK(oi->CANSystem->set(ARM_BASE_ID, 0));
    }

    return true;
}

bool HurricaneArmSystem::destroy() {
    HAL_ADC_Stop_IT(&CLAW_ADC);
    OK(this->arm_base->destroy());
    OK(this->arm_bottom->destroy());
    OK(this->arm_top->destroy());
    OK(oi->CANSystem->set(ARM_BOTTOM_ID, 0));
    OK(oi->CANSystem->set(ARM_BASE_ID, 0));
    return true;
}

bool HurricaneArmSystem::setPosition(double position) {
    this->position = position;
    this->arm_bottom->set_pos(this->arm_bottom->expected_pos((this->position - 0.5) * M_PI));
    this->arm_base->set_pos(this->arm_base->expected_pos((this->position - 0.5) * M_PI));
    return true;
}

void HurricaneArmSystem::data() {
    if (this->data_available_bottom ||
        (!this->data_available_bottom && oi->CANSystem->available(ARM_BOTTOM_ID, M2006_ANGLE_ID))) {
        this->data_available_bottom = true;
        this->arm_bottom->feed((int16_t) oi->CANSystem->get(ARM_BOTTOM_ID, M2006_ANGLE_ID),
                               (int16_t) oi->CANSystem->get(ARM_BOTTOM_ID, M2006_SPEED_ID));
    }
    if (this->data_available_base ||
        (!this->data_available_base && oi->CANSystem->available(ARM_BASE_ID, M3508_ANGLE_ID))) {
        this->data_available_base = true;
        this->arm_base->feed((int16_t) oi->CANSystem->get(ARM_BASE_ID, M3508_ANGLE_ID),
                             (int16_t) oi->CANSystem->get(ARM_BASE_ID, M3508_SPEED_ID));
    }
}

void HurricaneArmSystem::claw_trigger(uint32_t adc_value) {
    oi->debugSystem->info("CLA", std::to_string(adc_value));
}

void HURRICANE_CAN_0_5_6_DATA() {
    HSYS(armSystem) oi->armSystem->data();
}

extern "C" void Hurricane_HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    if (hadc == &CLAW_ADC) {
        HSYS(armSystem) oi->armSystem->claw_trigger(HAL_ADC_GetValue(&CLAW_ADC));
    }
}
