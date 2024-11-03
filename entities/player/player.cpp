#include "player.h"
#include "skills/skill_goTo.h"
#include "skills/skill_rotateTo.h"

quint8 Player::getPlayerId() {

    return _playerId;

}

VSSRef::Color Player::getPlayerColor() {

    return _playerColor;

}

float Player::getOrientation() {

    return _orientation;

}

float Player::getLinearSpeed() {
    
    // Implement
    return 0;

}

void Player::goTo(QVector2D &targetCoordinates, ActuatorClient *actuator) {

    _skillManager = std::make_unique<GoTo>(this, targetCoordinates);

    _skillManager->runSkill(actuator);

}

void Player::rotateTo(QVector2D &targetCoordinates, ActuatorClient *actuator) {
    _skillManager = std::make_unique<RotateTo>(this, targetCoordinates);

    _skillManager->runSkill(actuator);
}

QVector2D Player::getCoordinates() {

    return _coordinates;
    
}