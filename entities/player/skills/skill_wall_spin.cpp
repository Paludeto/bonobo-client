#include "skill_wall_spin.h"
#include "player/player.h"
#include <iostream>

WallSpinSkill::WallSpinSkill(Player *player, bool spinClockwise)
    : SkillManager(player), _spinClockwise(spinClockwise), _isComplete(false) {
    _spinTimer.start();
    std::cout << "Player " << _player->getPlayerId() << ": Iniciando giro para descolar da parede." << std::endl;
}

void WallSpinSkill::runSkill(ActuatorClient *actuator) {
    if (_spinTimer.elapsed() >= SPIN_DURATION_MS) {
        actuator->sendCommand(_player->getPlayerId(), 0.0f, 0.0f);
        _isComplete = true;
        return;
    }

    float leftWheelSpeed = _spinClockwise ? SPIN_SPEED : -SPIN_SPEED;
    float rightWheelSpeed = _spinClockwise ? -SPIN_SPEED : SPIN_SPEED;

    actuator->sendCommand(_player->getPlayerId(), leftWheelSpeed, rightWheelSpeed);
}

bool WallSpinSkill::isComplete() const {
    return _isComplete;
}