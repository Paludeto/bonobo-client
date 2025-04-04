#include "block_attack_behavior.h"
#include "basics/basic.h"
#include <iostream>

BlockAttackBehavior::BlockAttackBehavior(Player *player, WorldMap *worldMap,
                                     float ownGoalX, float ownGoalY,
                                     float opponentGoalX, float opponentGoalY)
    : Behavior(player, worldMap),
      _ownGoalX(ownGoalX),
      _ownGoalY(ownGoalY),
      _opponentGoalX(opponentGoalX),
      _opponentGoalY(opponentGoalY)
{
    _lastBallPos = _worldMap->getBallPosition();
}

void BlockAttackBehavior::execute(ActuatorClient *actuator) {
   // implement
}

bool BlockAttackBehavior::shouldActivate() {
    // Get team colors
    VSSRef::Color ourTeam = _player->getPlayerColor();
    
    // Check if ball is in our half
    bool ballInOurHalf = _worldMap->isBallInOurSide(ourTeam);
    
    // Check if opponent has the ball
    bool opponentHasBall = isOpponentWithBall();
    
    // Check if we're closest to ball
    bool isClosest = isClosestToBall();
    
    // Don't activate for goalkeeper
    if (_player->getPlayerId() == 0) {
        return false;
    }
    
    // Activate if ball is in our half or opponent has the ball
    return ballInOurHalf || opponentHasBall;
}

bool BlockAttackBehavior::shouldKeepActive() {
    // Use same criteria as activation
    return shouldActivate();
}

int BlockAttackBehavior::getPriority() const {
    // Medium-high priority for defense
    VSSRef::Color ourTeam = _player->getPlayerColor();
    bool ballInOurHalf = _worldMap->isBallInOurSide(ourTeam);
    
    // Higher priority when ball is in our half
    return ballInOurHalf ? 80 : 70;
}

bool BlockAttackBehavior::isOpponentWithBall() const {
    // Get positions
    QVector2D ballPos = _worldMap->getBallPosition();
    
    // Get opponent team
    VSSRef::Color ourTeam = _player->getPlayerColor();
    VSSRef::Color opponentTeam = (ourTeam == VSSRef::Color::BLUE) ? VSSRef::Color::YELLOW : VSSRef::Color::BLUE;
    
    // Get closest opponent to ball
    Player* closestOpponent = _worldMap->getPlayerClosestToBall(opponentTeam);
    
    if (closestOpponent) {
        // Check if opponent is close enough to be considered "with the ball"
        QVector2D opponentPos = closestOpponent->getCoordinates();
        float distance = Basic::getDistance(opponentPos, ballPos);
        
        return distance < BALL_POSSESSION_DIST;
    }
    
    return false;
}

bool BlockAttackBehavior::isClosestToBall() const {
    // Get our team color
    VSSRef::Color ourTeam = _player->getPlayerColor();
    
    // Get closest player on our team
    Player* closestPlayer = _worldMap->getPlayerClosestToBall(ourTeam);
    
    // Check if we are the closest
    return closestPlayer == _player;
}

QVector2D BlockAttackBehavior::calculateInterceptPosition() const {
    // Get positions
    QVector2D ballPos = _worldMap->getBallPosition();
    QVector2D playerPos = _player->getCoordinates();
    QVector2D ourGoal(_ownGoalX, _ownGoalY);
    
    // Determine if the ball is in our half
    VSSRef::Color ourTeam = _player->getPlayerColor();
    bool inOurHalf = _worldMap->isBallInOurSide(ourTeam);
    
    if (inOurHalf) {
        // Defensive position - between ball and our goal
        QVector2D goalToBall = ballPos - ourGoal;
        
        if (goalToBall.length() > 0.01f) {
            goalToBall.normalize();
            
            // Position partway between ball and goal
            float distanceToGoal = Basic::getDistance(ballPos, ourGoal);
            float positionFactor = 0.05f; // Position 20% of the way from ball to goal
            
            QVector2D interceptPos = ballPos - goalToBall * (distanceToGoal * positionFactor);
            
            // If ball is very close to our goal, go directly to it
            if (distanceToGoal < 0.3f) {
                return ballPos;
            }
            
            return interceptPos;
        }
    }
    
    // When ball is in opponent half or calculation fails, position directly at ball
    return ballPos;
}