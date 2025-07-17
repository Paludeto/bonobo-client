#include "role.h"
#include "player/skills/skill_wall_spin.h"
#include <iostream>

void Role::executeRole(ActuatorClient *actuator) {
    QVector2D currentPos = _player->getCoordinates();
    
    float wallMargin = _worldMap->getRobotRadius() * 2.0f + 0.05f;

    bool isNearYWall = std::abs(currentPos.y()) > (_worldMap->getFieldWidth() / 2.0f) - wallMargin;
    bool isNearXWall = std::abs(currentPos.x()) > (_worldMap->getFieldLength() / 2.0f) - wallMargin;

    bool isNearWall = isNearYWall || isNearXWall;


    if (Basic::getDistance(currentPos, _lastPosition) < 0.015f) { 
        if (isNearWall && !_stuckTimer.isValid()) { 
            _stuckTimer.start();
        }
    } else { 
        _stuckTimer.invalidate();
        _lastPosition = currentPos;
    }

    if (isNearWall && _stuckTimer.isValid() && _stuckTimer.elapsed() > 1000) {
        if (!_wallSpinSkill) {
            bool spinClockwise = (currentPos.y() > 0); // Gira para o lado que o afasta da parede
            _wallSpinSkill = std::make_unique<WallSpinSkill>(_player, spinClockwise);
            _isStuckOnWall = true;
            std::cout << "Player " << _player->getPlayerId() << " (Role) - PRESO! Ativando WallSpinSkill." << std::endl;
        }
    }

    if (_isStuckOnWall && _wallSpinSkill) {
        _wallSpinSkill->runSkill(actuator);

        if (_wallSpinSkill->isComplete()) {
            _wallSpinSkill.reset();
            _isStuckOnWall = false;
            _stuckTimer.invalidate();
        }
    }

    updateRoleState();
    int selectedBehaviorIndex = selectBehavior();

    if (selectedBehaviorIndex != _activeBehaviorIndex) {
        if (_activeBehaviorIndex >= 0 && _activeBehaviorIndex < _behaviors.size()) {
            _behaviors[_activeBehaviorIndex]->deactivate();
        }
        _activeBehaviorIndex = selectedBehaviorIndex;
        if (_activeBehaviorIndex >= 0 && _activeBehaviorIndex < _behaviors.size()) {
            _behaviors[_activeBehaviorIndex]->activate();
        }
    }

    if (_activeBehaviorIndex >= 0 && _activeBehaviorIndex < _behaviors.size()) {
        _behaviors[_activeBehaviorIndex]->execute(actuator);
    }
}

Behavior *Role::getActiveBehavior() {
    if (_activeBehaviorIndex >= 0 && _activeBehaviorIndex < _behaviors.size()) {
        return _behaviors[_activeBehaviorIndex].get();
    }
    return nullptr;
}

int Role::selectBehavior() {
    int selectedIndex = -1;
    int highestPriority = -1;

    // First check if current behavior should remain active
    if (_activeBehaviorIndex >= 0 && _activeBehaviorIndex < _behaviors.size()) {
        Behavior* currentBehavior = _behaviors[_activeBehaviorIndex].get();
        if (currentBehavior->shouldKeepActive()) {
            return _activeBehaviorIndex;
        }
    }

    // Find the highest priority behavior that should be activated
    for (size_t i = 0; i < _behaviors.size(); i++) {
        Behavior* behavior = _behaviors[i].get();
        
        if (behavior->shouldActivate() && behavior->getPriority() > highestPriority) {
            highestPriority = behavior->getPriority();
            selectedIndex = static_cast<int>(i);
        }
    }
    
    return selectedIndex;
}