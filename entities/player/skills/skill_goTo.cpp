#include "skill_goTo.h"
#include "player/player.h"

void GoTo::goTo(QVector2D &targetCoordinates, ActuatorClient *actuator) {
    static float lastError = 0;
    float robotAngle = _player->getOrientation();
    float targetAngle = Basic::getAngle(_player->getCoordinates(), targetCoordinates);

    // Check if reversing is optimal
    bool reversed = Basic::checkCanBeReversed(robotAngle, targetAngle);
    if (reversed) {
        robotAngle = Basic::normalizeAngle(robotAngle + M_PI);
    }

    // Calculate angle error and motor speed using proportional control (P)
    float angleError = Basic::smallestAngleDiff(robotAngle, targetAngle);
    float motorSpeed = std::clamp(KP * angleError + KD * (angleError - lastError), -BASE_SPEED, BASE_SPEED);
    lastError = angleError;

    // Calculate left and right wheel speeds based on reversed state
    float baseSpeed = reversed ? -BASE_SPEED : BASE_SPEED;
    float leftWheelSpeed = baseSpeed + (motorSpeed < 0 ? motorSpeed : 0);
    float rightWheelSpeed = baseSpeed - (motorSpeed > 0 ? motorSpeed : 0);

    // Send command to actuator
    actuator->sendCommand(_player->getPlayerId(), leftWheelSpeed, rightWheelSpeed);
}

void GoTo::runSkill(ActuatorClient *actuator) {

    goTo(_targetCoordinates, actuator);

}