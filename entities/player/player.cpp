#include "player.h"
#include "skills/skill_goTo.h"

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

void Player::goTo(QVector2D &targetCoordinates) {

    _skillManager = std::make_unique<GoTo>(this, targetCoordinates);

    _skillManager->runSkill();

}

QVector2D Player::getCoordinates() {

    return _coordinates;
    
}