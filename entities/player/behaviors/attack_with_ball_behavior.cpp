#include "attack_with_ball_behavior.h"
#include "basics/basic.h"
#include <iostream>

AttackWithBallBehavior::AttackWithBallBehavior(Player *player, WorldMap *worldMap,
                                         float ownGoalX, float ownGoalY,
                                         float opponentGoalX, float opponentGoalY)
    : Behavior(player, worldMap),
      _ownGoalX(ownGoalX),
      _ownGoalY(ownGoalY),
      _opponentGoalX(opponentGoalX),
      _opponentGoalY(opponentGoalY),
      _avoidDefenseArea(false),
      _hadBallPossession(false)
{
}

void AttackWithBallBehavior::execute(ActuatorClient *actuator) {
    // Get positions
    QVector2D ballPos = _worldMap->getBallPosition();
    QVector2D playerPos = _player->getCoordinates();
    QVector2D opponentGoal(_opponentGoalX, _opponentGoalY);
    
    // Check if we have the ball
    bool haveBall = hasBallPossession();
    if (haveBall) {
        _hadBallPossession = true;
        
        // Calculate angle to opponent goal
        float angleToGoal = Basic::getAngle(playerPos, opponentGoal);
        
        // Get current angle
        float currentAngle = _player->getOrientation();
        
        // Calculate angle difference
        float angleDiff = Basic::smallestAngleDiff(currentAngle, angleToGoal);
        
        // If angle is close enough, drive forward, otherwise turn
        if (std::abs(angleDiff) < 0.3f) {
            // Aligned well enough, drive forward
            _player->pathPlanning(opponentGoal, _worldMap, _worldMap->getRobotRadius(), actuator);
        } else {
            // Need to turn
            if (angleDiff > 0) {
                actuator->sendCommand(_player->getPlayerId(), 60.0f, 20.0f); // Turn right
            } else {
                actuator->sendCommand(_player->getPlayerId(), 20.0f, 60.0f); // Turn left
            }
        }
    } 
    else {
        // Don't have the ball, go get it
        float distToBall = Basic::getDistance(playerPos, ballPos);
        
        // Calculate position behind ball relative to goal
        QVector2D ballToGoal = opponentGoal - ballPos;
        if (ballToGoal.length() > 0.01f) {
            ballToGoal.normalize();
            
            // Position behind ball to approach from proper angle
            QVector2D targetPos = ballPos - ballToGoal * 0.08f;
            
            // Use path planning to reach the position
            _player->pathPlanning(targetPos, _worldMap, _worldMap->getRobotRadius(), actuator);
        }
        else {
            // Just go to the ball if calculation fails
            _player->pathPlanning(ballPos, _worldMap, _worldMap->getRobotRadius(), actuator);
        }
    }
}

bool AttackWithBallBehavior::shouldActivate() {
    // Activate if we have the ball or are closest to it on our team
    bool haveBall = hasBallPossession();
    
    // Don't activate for goalkeeper
    if (_player->getPlayerId() == 0) {
        return false;
    }
    
    // Check if we're closest to ball
    VSSRef::Color ourTeam = _player->getPlayerColor();
    Player* closestPlayer = _worldMap->getPlayerClosestToBall(ourTeam);
    bool isClosest = (closestPlayer == _player);
    
    return haveBall || (isClosest && !_worldMap->isBallInOurSide(ourTeam));
}

bool AttackWithBallBehavior::shouldKeepActive() {
    // Keep active slightly longer than activation to avoid oscillation
    if (_hadBallPossession) {
        VSSRef::Color ourTeam = _player->getPlayerColor();
        Player* closestPlayer = _worldMap->getPlayerClosestToBall(ourTeam);
        bool isClosest = (closestPlayer == _player);
        
        // If we had the ball and are still closest, keep active
        if (isClosest) {
            return true;
        }
        
        // Reset possession state
        _hadBallPossession = false;
    }
    
    return shouldActivate();
}

int AttackWithBallBehavior::getPriority() const {
    // High priority when we have the ball
    return hasBallPossession() ? 100 : 90;
}

bool AttackWithBallBehavior::hasBallPossession() const {
    // Simply check distance to ball
    QVector2D ballPos = _worldMap->getBallPosition();
    QVector2D playerPos = _player->getCoordinates();
    
    float distanceToBall = Basic::getDistance(playerPos, ballPos);
    
    // Use WorldMap method if available, otherwise use distance
    if (_worldMap->isPlayerControllingBall(_player)) {
        return true;
    }
    
    return distanceToBall < BALL_POSSESSION_DIST;
}