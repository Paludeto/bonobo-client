#include "skill_goTo.h"
#include "player/player.h"

void GoTo::goTo(QVector2D &targetCoordinates, ActuatorClient *actuator) {

    float robotAngle = _player->getOrientation();
    float targetAngle = Basic::getAngle(_player->getCoordinates(), targetCoordinates);

    float error = 10 * (targetAngle - robotAngle);

    actuator->setTeamColor(_player->getPlayerColor());
    actuator->sendCommand(_player->getPlayerId(), 0, error);

} 

void GoTo::runSkill(ActuatorClient *actuator) {

    goTo(_targetCoordinates, actuator);

}