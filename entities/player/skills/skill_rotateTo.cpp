#include "skill_rotateTo.h"
#include "player/player.h"

void RotateTo::rotateTo(QVector2D &targetCoordinates, ActuatorClient *actuator) {
    static float lastError = 0;
    float targetAngle = Basic::getAngle(_player->getCoordinates(), targetCoordinates);
    float robotAngle = _player->getOrientation();

    // Check if reversing is optimal
    float reversed = Basic::checkCanBeReversed(robotAngle, targetAngle);
    if(reversed) {
        robotAngle = Basic::normalizeAngle(robotAngle + M_PI);
    }

    // Calculate angle error and motor speed using proportional and derivative control (PD)
    float angleError = Basic::smallestAngleDiff(robotAngle, targetAngle);
    float motorSpeed = (KP * angleError) + (KD * (angleError - lastError));
    lastError = angleError;

    // Calculate left and right wheel speeds based on reversed state, rotating clockwise and counterclockwise.
    float leftWheelSpeed = reversed ? -motorSpeed : motorSpeed;
    float rightWheelSpeed = reversed ? motorSpeed : -motorSpeed;

    actuator->sendCommand(_player->getPlayerId(), leftWheelSpeed, rightWheelSpeed);
}



void RotateTo::runSkill(ActuatorClient *actuator) {
    rotateTo(_targetCoordinates, actuator);
}