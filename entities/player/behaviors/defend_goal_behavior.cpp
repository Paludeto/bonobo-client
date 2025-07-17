#include "defend_goal_behavior.h"
#include "basics/basic.h"
#include <cmath>
#include <iostream>

DefendGoalBehavior::DefendGoalBehavior(Player *player, WorldMap *worldMap, float ownGoalX, float ownGoalY)
    : Behavior(player, worldMap),
      _state(STATE_GOTO),
      _ownGoalX(ownGoalX),
      _ownGoalY(ownGoalY)
{

}

void DefendGoalBehavior::execute(ActuatorClient *actuator) {
    // Switch state
    switch (_state) {
        default:
        case STATE_GOTO: {


        } break;
        
        case STATE_SHORTTAKEOUT: {
            // In takeout state, we push the ball away from our goal
            
            // Get positions
            QVector2D ballPos = _worldMap->getBallPosition();
            QVector2D robotPos = _player->getCoordinates();
            
            // Move to push ball away
            _player->goTo(ballPos, actuator);
            
            ballPos = _worldMap->getBallPosition();
            robotPos = _player->getCoordinates();

            float distanceToBall = Basic::getDistance(robotPos, ballPos);

            if(distanceToBall < 0.08) {
                actuator->sendCommand(_player->getPlayerId(), 100, -100);
            }
            
            // Check if we should switch back to normal positioning
            if (!isInsideOurArea(ballPos, TAKEOUT_FACTOR_OUT)) {
                std::cout << "DefendGoalBehavior: Ball left our area, returning to normal positioning" << std::endl;
                _state = STATE_GOTO;
            }
        } break;
    }
}

bool DefendGoalBehavior::shouldActivate() {
    // Goalkeeper's behavior should activate when:
    // 1. Player ID is 0 (goalkeeper)
    // 2. Ball is in our half or coming to our goal
    
    // Check if this player is the goalkeeper (assuming player 0 is goalkeeper)
    if (_player->getPlayerId() != 0) {
        return false;
    }
    
    // Get our team color
    VSSRef::Color ourTeam = _player->getPlayerColor();
    
    // Check if ball is in our half
    bool ballInOurHalf = _worldMap->isBallInOurSide(ourTeam);
    
    // Or if ball is coming to our goal
    bool ballComingToGoal = isBallComingToGoal(POSTSFACTOR);
    
    return true;
}

bool DefendGoalBehavior::shouldKeepActive() {
    // Once activated, goalkeeper behavior should remain active
    return true;
}

int DefendGoalBehavior::getPriority() const {
    // Goalkeeper defense has highest priority
    return 100;
}

QVector2D DefendGoalBehavior::getFollowBallPos() {
    // Get our team color and the ball position
    VSSRef::Color teamColor = _player->getPlayerColor();
    QVector2D ballPos = _worldMap->getBallPosition();
    QVector2D goalPos = QVector2D(_ownGoalX, _ownGoalY);
    
    float signal = _ownGoalX > 0 ? -1 : 1;
    float posX = (_ownGoalX + signal * (_worldMap->getRobotRadius() + 0.075)/ 2);

    float posY;
    if(_worldMap->isBallInTheirSide(_player->getPlayerColor())) {
        posY = 0;
    } else if((_worldMap->isBallInOurSide(_player->getPlayerColor()) && ballPos.x() <= _worldMap->getMaxX() * 3/4) || (_worldMap->isBallInOurSide(_player->getPlayerColor()) && ballPos.x() >= _worldMap->getMinX() * 3/4)) {
        posY = (fabs(_worldMap->getOurRightPost(_player->getPlayerColor()).y() / fabs(_worldMap->getMaxY())) * ballPos.y());
    } else {
        if(ballPos.y() > fabs(_worldMap->getOurRightPost(_player->getPlayerColor()).y())) {
            posY = fabs(_worldMap->getOurRightPost(_player->getPlayerColor()).y()) - _worldMap->getRobotRadius() / 2;
        } else if(ballPos.y() < -fabs(_worldMap->getOurRightPost(_player->getPlayerColor()).y())) {
            posY = -fabs(_worldMap->getOurRightPost(_player->getPlayerColor()).y()) + _worldMap->getRobotRadius() / 2;
        } else {
            posY = ballPos.y();
        }
    }

    return QVector2D(posX, posY);
}

bool DefendGoalBehavior::isBallComingToGoal(float postsFactor) {
    QVector2D ballPos = _worldMap->getBallPosition();
    QVector2D posRightPost(_worldMap->getOurRightPost(_player->getPlayerColor()).x(), _worldMap->getOurRightPost(_player->getPlayerColor()).y() * postsFactor);
    QVector2D posLeftPost(_worldMap->getOurLeftPost(_player->getPlayerColor()).x(), _worldMap->getOurLeftPost(_player->getPlayerColor()).y() * postsFactor);
    QVector2D ballVelocity = _worldMap->getBallVelocity();

    if(ballVelocity.length() < BALL_MINVELOCITY) {
        return false;
    }
    
    float angVel = std::atan2(ballVelocity.y(), ballVelocity.x());
    float angRightPost = Basic::getAngle(ballPos, posRightPost);
    float angLeftPost = Basic::getAngle(ballPos, posLeftPost);
    float angleDiffPosts = Basic::smallestAngleDiff(angRightPost, angLeftPost);

    float angDiffRight = fabs(Basic::smallestAngleDiff(angVel, angRightPost));
    float angDiffLeft = fabs(Basic::smallestAngleDiff(angVel, angLeftPost));

    return (angDiffRight < angleDiffPosts && angDiffLeft < angleDiffPosts);
}

// TODO : Correct factors and consts to take ball right
bool DefendGoalBehavior::isInsideOurArea(const QVector2D& point, float factor) {
    if(point.x() > 0.5 && point.y() < 0.4 && point.y() > -0.4) {
        return true;
    }
    else {
        return false;
    }
}