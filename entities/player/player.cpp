#include "player.h"

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

QVector2D Player::getCoordinates() {

    return _coordinates;
    
}