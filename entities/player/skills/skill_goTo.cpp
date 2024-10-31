#include "skill_goTo.h"
#include "player/player.h"

void GoTo::goTo(QVector2D &targetCoordinates) {

    float robotAngle = _player->getOrientation();
    float targetAngle = Basic::getAngle(_player->getCoordinates(), targetCoordinates);

    float error = 10 * (targetAngle - robotAngle);

    _player->_actuator->setTeamColor(_player->getPlayerColor());
    _player->_actuator->sendCommand(_player->getPlayerId(), 0, error);

} 

void GoTo::runSkill() {

    goTo(_targetCoordinates);

}