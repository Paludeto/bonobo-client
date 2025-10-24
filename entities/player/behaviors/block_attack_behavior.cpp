#include "block_attack_behavior.h"
#include "basics/basic.h"
#include <cmath>
#include <iostream>

BlockAttackBehavior::BlockAttackBehavior(Player *player, WorldMap *worldMap,
                                     float ownGoalX, float ownGoalY,
                                     float opponentGoalX, float opponentGoalY)
    : Behavior(player, worldMap),
      _ownGoalX(ownGoalX),
      _ownGoalY(ownGoalY),
      _opponentGoalX(opponentGoalX),
      _opponentGoalY(opponentGoalY),
      _state(TAKE_BALL_STATE)
{
    _lastBallPos = _worldMap->getBallPosition();
}

void BlockAttackBehavior::execute(ActuatorClient *actuator) {
    switch(_state) {
        case TAKE_BALL_STATE: {
            QVector2D ballPos = _worldMap->getBallPosition();
            Player *closer = _worldMap->getPlayerClosestToBall(_player->getPlayerColor());

            if (closer == _player) {
                QVector2D target = calculateInterceptPosition();

                if (_worldMap->isInsideOurArea(target, 1.5f, _player->getPlayerColor())) {
                    QVector2D goal((_player->getPlayerColor() == VSSRef::BLUE)
                                    ? _worldMap->getMinX() : _worldMap->getMaxX(),
                                    0.0f);

                    QVector2D direction = target - goal;
                    float dirLenSq = direction.lengthSquared();
                    if (dirLenSq > 1e-6f) {
                        direction /= std::sqrt(dirLenSq);
                        target = goal + direction * (_worldMap->getAreaLength() * 0.3f);
                    } else {
                        direction = QVector2D((_player->getPlayerColor() == VSSRef::BLUE) ? 1.0f : -1.0f, 0.0f);
                        target = goal + direction * (_worldMap->getAreaLength() * 0.3f);
                    }
                }

                _player->goTo(target, actuator);
            } else {
                _state = POSITIONING_STATE;
            }
        } break;

        case POSITIONING_STATE: {
            QVector2D desiredPos = calculateBestPosition();
            _player->univector(desiredPos, _worldMap, _worldMap->getRobotRadius(), actuator);

            Player *player = _worldMap->getPlayerClosestToBall(_player->getPlayerColor());
            if (player == _player) {
                _state = TAKE_BALL_STATE;
            }
        } break;
    }
}

bool BlockAttackBehavior::shouldActivate() {
    return true;
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
    QVector2D ourGoal(_ownGoalX, _ownGoalY);
    
    // Determine if the ball is in our half
    VSSRef::Color ourTeam = _player->getPlayerColor();
    bool inOurHalf = _worldMap->isBallInOurSide(ourTeam);
    
    if (inOurHalf) {
        // Defensive position - between ball and our goal
        QVector2D goalToBall = ballPos - ourGoal;

        if (goalToBall.lengthSquared() > 1e-6f) {
            goalToBall.normalize();

            // Position partway between ball and goal
            float distanceToGoal = Basic::getDistance(ballPos, ourGoal);
            float positionFactor = 0.2f; // Position 20% of the way from ball to goal

            QVector2D interceptPos = ballPos - goalToBall * (distanceToGoal * positionFactor);

            return interceptPos;
        }
    }
    
    // When ball is in opponent half or calculation fails, position directly at ball
    return ballPos;
}

QVector2D BlockAttackBehavior::calculateBestPosition() {
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