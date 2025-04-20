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
                
            } 

            Player *player = _worldMap->getPlayerClosestToBall(_player->getPlayerColor());

            if(player == _player) {
                _state = TAKE_BALL_STATE;
            } else {
                _state = POSITIONING_STATE;
            }
            
        } break;

        case TAKE_BALL_STATE: {
            QVector2D ballPos = _worldMap->getBallPosition();
            QVector2D opponentGoal(_opponentGoalX, _opponentGoalY);

            QVector2D direction = opponentGoal - ballPos;
            direction.normalize();

            QVector2D behindBallPos = ballPos - direction * 0.1f;

            _player->pathPlanning(ballPos, _worldMap, _worldMap->getRobotRadius(), actuator);
            
            float distanceToBall = Basic::getDistance(_player->getCoordinates(), ballPos);
            
            if(distanceToBall < 0.08) {
                _state = STATE_ATTACK;
            }
        } break;

        case POSITIONING_STATE: {
            QVector2D opponentGoal(_opponentGoalX, _opponentGoalY);
            QVector2D ballPos = _worldMap->getBallPosition();
            

            QVector2D desiredPos = calculateBestPosition();

            _player->pathPlanning(desiredPos, _worldMap, _worldMap->getRobotRadius(), actuator);

            Player *player = _worldMap->getPlayerClosestToBall(_player->getPlayerColor());
 
            if(player == _player) {
                _state = TAKE_BALL_STATE;
            } else {
                _state = POSITIONING_STATE;
            }
            
        } break;
    }
}

bool AttackWithBallBehavior::shouldActivate() {
    
    return true;
}

bool AttackWithBallBehavior::shouldKeepActive() {
    
    
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

QVector2D AttackWithBallBehavior::calculateBestPosition() {
    std::vector<QVector2D> points;
    QList<Player*> ourTeam = _worldMap->getTeam(_player->getPlayerColor());
    VSSRef::Color theirColor = (_player->getPlayerColor() == VSSRef::BLUE) ? VSSRef::YELLOW : VSSRef::BLUE; 

    QList<Player*> theirTeam = _worldMap->getTeam(theirColor);

    for(Player* player : ourTeam) {
        if(_player->getPlayerId() == player->getPlayerId()) continue;
        if(player->getPlayerId() == 0) continue;
        points.push_back(player->getCoordinates());
    }

    for(Player* player : theirTeam) {
        points.push_back(player->getCoordinates());
    }

    std::vector<std::vector<QVector2D>> triangles = Basic::triangularization(points);
    
    float maxRadius = -1;
    QVector2D bestPoint;

    for(std::vector<QVector2D> t : triangles) {
        std::cout << "Ado" << std::endl;
        QVector2D center = Basic::getCircumcenter(t);
        
        std::cout << center.x() << center.y() << std::endl;
        
        float minDist = std::numeric_limits<float>::max();

        for(const QVector2D p : points) {
            float dx = p.x() - center.x();
            float dy = p.y() - center.y();
            float dist = std::sqrt(dx*dx + dy*dy);
            minDist = std::min(minDist, dist);
        }

        if(minDist > maxRadius) {
            maxRadius = minDist;
            bestPoint = center;
        }
    }

    return bestPoint;
}