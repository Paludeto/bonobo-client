#include "player.h"
#include "skills/skill_goTo.h"
#include "skills/skill_rotateTo.h"
#include "skills/rrt/skill_rrt.h"

quint8 Player::getPlayerId() {
    return _playerId;
}

VSSRef::Color Player::getPlayerColor() {
    return _playerColor;
}

float &Player::getOrientation() {
    return _orientation;
}

float Player::getLinearSpeed() {
    // Calculate linear speed from velocity components _vX and _vY
    return std::sqrt(_vX * _vX + _vY * _vY);
}

QVector2D Player::getVelocity() {
    return QVector2D(_vX, _vY);
}

void Player::goTo(QVector2D targetCoordinates, ActuatorClient *actuator) {
    _skillManager = std::make_unique<GoTo>(this, targetCoordinates);
    _skillManager->runSkill(actuator);
}

void Player::rotateTo(float angle, ActuatorClient *actuator) {
    _skillManager = std::make_unique<RotateTo>(this, angle);
    _skillManager->runSkill(actuator);
}

QVector2D Player::getCoordinates() {
    return _coordinates;
}

void Player::pathPlanning(QVector2D& targetPosition, WorldMap *worldMap, float robotRadius, ActuatorClient *actuator) {
    // Check if we already have an RRT instance or if the target has changed
    bool needNewRRT = false;
    
    // First case: we don’t have any skill manager
    if (!_skillManager) {
        needNewRRT = true;
    } 
    // Second case: we have a skill manager, but it’s not an RRT
    else if (typeid(*_skillManager) != typeid(RRT)) {
        needNewRRT = true;
    }
    // Third case: we have an RRT, but the target has changed significantly
    else {
        RRT* rrt = static_cast<RRT*>(_skillManager.get());
        QVector2D currentTarget = rrt->getTargetPosition();
        
        // Check if the target has changed enough to justify a new RRT
        // Use a small margin (0.02m) to avoid unnecessary recreations due to minor changes
        float targetDiff = (currentTarget - targetPosition).length();
        if (targetDiff > 0.02f) {
            needNewRRT = true;
        }
    }
    
    // Create a new RRT instance only if needed
    if (needNewRRT) {
        // Pass the stuck counter as a parameter to the new RRT
        _skillManager = std::make_unique<RRT>(this, targetPosition, worldMap, robotRadius, _rrtStuckCounter);
    } 
    
    // Run the skill
    _skillManager->runSkill(actuator);
    
    // Update the player’s stuck counter
    if (typeid(*_skillManager) == typeid(RRT)) {
        RRT* rrt = static_cast<RRT*>(_skillManager.get());
        _rrtStuckCounter = rrt->getStuckCounter();
    }
}