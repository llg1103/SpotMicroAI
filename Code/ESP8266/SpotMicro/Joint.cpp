/*
 * Joint.cpp
 *
 *  ******************************************************************************************************************************
 *  Class implementing a single joint of SpotMicro (aka a single Servo)
 *  *****************************************************************************************************************************
 */

#include "Joint.h"
#include <Arduino.h>

Joint::Joint()
{
    Servo = ServoEasing(PCA9685_DEFAULT_ADDRESS, &Wire);
}

bool Joint::init(int pin, int min, int max, int off, bool invert)
{
    this->Servo.attach(pin);
    servomin = min;
    servomax = max;
    offset = off;
    this->invert = invert;
    this->Servo.setEasingType(EASE_SINE_IN_OUT);
}

bool Joint::moveAngle(int angle, int msec)
{
    int targetangle;

    if (invert == true)
    {
        targetangle = 180 - angle;
    }
    else
    {
        targetangle = angle;
    }
    if (angle < servomin || angle > servomax)
    {
        Serial.print("invalid target angle ");
        Serial.println(angle);
        return false;
    }

    Serial.print("move to ");
    Serial.println(targetangle);
    this->Servo.startEaseToD(targetangle, msec);
    isLastMoveComplete = false;
    while (this->Servo.isMovingAndCallYield())
    {
        ; // no delays here to avoid break between forth and back movement
    }
    isLastMoveComplete = true;
    return true;
}

int Joint::getServoMin()
{
    return this->servomin;
}
int Joint::getServoMax()
{
    return this->servomax;
}

bool Joint::lastMoveComplete()
{
    return this->isLastMoveComplete;
}
