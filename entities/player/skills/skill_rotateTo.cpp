#include "skill_rotateTo.h"
#include "player/player.h"

void RotateTo::rotateTo(float angle, ActuatorClient *actuator) {
    static float lastError = 0;
    float targetAngle = angle;
    float robotAngle = _player->getOrientation();

    // Check if reversing is optimal
    float reversed = Basic::checkCanBeReversed(robotAngle, targetAngle);
    if(reversed) {
        robotAngle = Basic::normalizeAngle(robotAngle + M_PI);
    }

    // Calculate angle error and motor speed using proportional and derivative control (PD)
    float angleError = Basic::smallestAngleDiff(robotAngle, targetAngle);
    if (fabs(angleError) < 0.087f) {
        actuator->sendCommand(_player->getPlayerId(), 0.0f, 0.0f);
        lastError = 0;  
        return;
    }
    float motorSpeed = (KP * angleError) + (KD * (angleError - lastError));
    lastError = angleError;

    // Calculate left and right wheel speeds based on reversed state, rotating clockwise and counterclockwise.
    float leftWheelSpeed = reversed ? -motorSpeed : motorSpeed;
    float rightWheelSpeed = reversed ? motorSpeed : -motorSpeed;

    actuator->sendCommand(_player->getPlayerId(), leftWheelSpeed, rightWheelSpeed);
}



void RotateTo::runSkill(ActuatorClient *actuator) {
    rotateTo(_angle, actuator);
}