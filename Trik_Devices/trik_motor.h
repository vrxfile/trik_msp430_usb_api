/*
 * Trik_Motor.h
 *
 *  Created on: October 21, 2014
 *      Author: Rostislav Varzar
 */
#ifndef TRIK_MOTOR_H_
#define TRIK_MOTOR_H_

//Configuration parameters
#define DISABLE				0
#define ENABLE				1

//Prototypes for the APIs
uint8_t MOTOR_hadler(uint8_t MOT_NUMBER);
void MOTOR_enableController(uint8_t MOT_NUMBER);
void MOTOR_disableController(uint8_t MOT_NUMBER);
//void MOTOR_setContinuesMode(uint8_t MOT_NUMBER);
//void MOTOR_setSingleTimeMode(uint8_t MOT_NUMBER);
//void MOTOR_setSingleAngleMode(uint8_t MOT_NUMBER);
//void MOTOR_enableFeedBackSensor(uint8_t MOT_NUMBER);
//void MOTOR_disableFeedBackSensor(uint8_t MOT_NUMBER);
//void MOTOR_set1wireSensor(uint8_t MOT_NUMBER);
//void MOTOR_set2wireSensor(uint8_t MOT_NUMBER);
//void MOTOR_setAnalogSensor(uint8_t MOT_NUMBER);
//void MOTOR_setSensorPort(uint8_t MOT_NUMBER, uint8_t SENSPORT_NUMBER);
//void MOTOR_resetFeedBackSensor(uint8_t MOT_NUMBER);
void MOTOR_rotationForward(uint8_t MOT_NUMBER);
void MOTOR_rotationBackward(uint8_t MOT_NUMBER);
void MOTOR_enableBrake(uint8_t MOT_NUMBER);
void MOTOR_disableBrake(uint8_t MOT_NUMBER);
void MOTOR_fastBrake(uint8_t MOT_NUMBER);
//uint32_t MOTOR_getSensorValue(uint8_t MOT_NUMBER);
//uint8_t MOTOR_isError(uint8_t MOT_NUMBER);
//uint8_t MOTOR_isOverPower(uint8_t MOT_NUMBER);
//uint8_t MOTOR_isWorking(uint8_t MOT_NUMBER);
void MOTOR_start(uint8_t MOT_NUMBER);
void MOTOR_stop(uint8_t MOT_NUMBER);

//Max motors
#define MAX_MOTORS 4

//Motor registers
struct tMotorRegisters
{
    uint16_t MCTL;
    uint16_t MPWR;
    uint16_t MFRQ;
    uint16_t MANG;
    uint16_t MTMR;
    uint32_t MVAL;
    uint16_t MSTA;
    uint8_t MOT_EN;
    uint8_t MOT_PWR;
    uint8_t MOT_DIR;
    uint8_t MOT_BRK;
};

//Motors registers array
struct tMotorRegisters MOT[MAX_MOTORS];

#endif
