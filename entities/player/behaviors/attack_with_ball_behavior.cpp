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
            if (hasBallPossession()) {
                QVector2D opponentGoal(_opponentGoalX, _opponentGoalY);

                QVector2D attackVector = opponentGoal - _player->getCoordinates();
                attackVector.normalize();

                QVector2D strategicTarget = opponentGoal + attackVector * 0.15;

                _player->univector(strategicTarget, _worldMap, _worldMap->getRobotRadius(), actuator);

                QVector2D ballPos = _worldMap->getBallPosition();
                float distanceToBall = Basic::getDistance(_player->getCoordinates(), ballPos);

                std::cout << "STATE_ATTACK" << std::endl;
                
                break;
            }

            // Se não tem posse, checa se é o mais próximo
            Player* player = _worldMap->getPlayerClosestToBall(_player->getPlayerColor());
            if (player == nullptr || player == _player) {
                _state = TAKE_BALL_STATE;
            } else {
                _state = POSITIONING_STATE;
            }
        } break;

        case TAKE_BALL_STATE: {
            QVector2D ballPos = _worldMap->getBallPosition();
            QVector2D opponentGoal(_opponentGoalX, _opponentGoalY);
            
            if(_worldMap->isBallInOurSide(_player->getPlayerColor())) {
                QVector2D direction = opponentGoal - ballPos;
                direction.normalize();

                QVector2D behindBallPos = ballPos - direction * 0.1f;
            
                _player->univector(behindBallPos, _worldMap, _worldMap->getRobotRadius(), actuator);

            } else {
                _player->goTo(ballPos, actuator);
            }

            float distanceToBall = Basic::getDistance(_player->getCoordinates(), ballPos);
            if (distanceToBall < 0.06f) {
                _state = STATE_ATTACK;
            }
        } break;

        case POSITIONING_STATE: {
            QVector2D desiredPos = calculateBestPosition();
            _player->univector(desiredPos, _worldMap, _worldMap->getRobotRadius(), actuator);

            Player* player = _worldMap->getPlayerClosestToBall(_player->getPlayerColor());
            if (player == _player) {
                _state = TAKE_BALL_STATE;
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
        QVector2D center = Basic::getCircumcenter(t, _worldMap);
        
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