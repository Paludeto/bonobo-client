#include "skill_goTo.h"
#include "player/player.h"

void GoTo::goTo(QVector2D &targetCoordinates, ActuatorClient *actuator) {

    float robotAngle = _player->getOrientation();
    float targetAngle = Basic::getAngle(_player->getCoordinates(), targetCoordinates);


    float angleError = targetAngle - robotAngle;
    float goalDistance = Basic::getDistance(_player->getCoordinates(), targetCoordinates);

    if (angleError > 0.1) {
        actuator->sendCommand(_player->getPlayerId(), 0, 10 * angleError);
    } else {
        actuator->sendCommand(_player->getPlayerId(), 10, 10);
    }

} 

void GoTo::runSkill(ActuatorClient *actuator) {

    goTo(_targetCoordinates, actuator);

}