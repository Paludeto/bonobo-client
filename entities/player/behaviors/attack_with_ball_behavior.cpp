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
      _hadBallPossession(false),
      _state(STATE_ATTACK)
{
}

void AttackWithBallBehavior::execute(ActuatorClient *actuator) {
    switch (_state) {
        case STATE_ATTACK: {
            QVector2D opponentGoal(_opponentGoalX, _opponentGoalY);
            QVector2D ballPos = _worldMap->getBallPosition();
            
            if(hasBallPossession()) {

                _player->pathPlanning(opponentGoal, _worldMap, _worldMap->getRobotRadius(), actuator);
                
            } else {
                _state = TAKE_BALL_STATE;
            }
            
        } break;

        case TAKE_BALL_STATE: {
            QVector2D ballPos = _worldMap->getBallPosition();
            QVector2D opponentGoal(_opponentGoalX, _opponentGoalY);

            QVector2D ballToTarget = opponentGoal - ballPos;
            ballToTarget.normalize();

            QVector2D behindBallPos = ballPos - ballToTarget * (_worldMap->getRobotRadius() + 0.01f);

            _player->pathPlanning(behindBallPos, _worldMap, _worldMap->getRobotRadius(), actuator);
            
            if(hasBallPossession()) {
                _state = STATE_ATTACK;
            }
        } break;
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
    
    return true;
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