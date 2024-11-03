#include "skill_rotateTo.h"
#include "player/player.h"

void RotateTo::rotateTo(QVector2D &targetCoordinates, ActuatorClient *actuator) {
    static float lastError = 0;
    float targetAngle = Basic::getAngle(_player->getCoordinates(), targetCoordinates);
    float robotAngle = _player->getOrientation();

    float reversed = Basic::checkCanBeReversed(robotAngle, targetAngle);
    if(reversed) {
        robotAngle = Basic::normalizeAngle(robotAngle + M_PI);
    }

    float angleError = Basic::smallestAngleDiff(robotAngle, targetAngle);
    float motorSpeed = (KP * angleError) + (KD * (angleError - lastError));
    lastError = angleError;

    float leftWheelSpeed = reversed ? -motorSpeed : motorSpeed;
    float rightWheelSpeed = reversed ? motorSpeed : -motorSpeed;

    actuator->sendCommand(_player->getPlayerId(), leftWheelSpeed, rightWheelSpeed);
}



void RotateTo::runSkill(ActuatorClient *actuator) {
    rotateTo(_targetCoordinates, actuator);
}