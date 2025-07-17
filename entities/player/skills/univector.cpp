#include "univector.h"
#include "player/player.h"
#include "worldmap/worldmap.h"
#include "basics/basic.h"
#include <QTransform>
#include <cmath>
#include <iostream>

QVector2D UnivectorField::getDirection(Player *player, const QVector2D& target, WorldMap *worldMap) {
    QVector2D robotPos = player->getCoordinates();
    QVector2D attractiveField = moveToGoalField(robotPos, target, worldMap, player);
    QVector2D repulsiveField = avoidObstacleField(robotPos, player, worldMap, attractiveField);

    if (repulsiveField.length() < 1e-6) {
        return attractiveField.normalized();
    }
    
    float minDistance = std::numeric_limits<float>::max();
    auto allPlayers = worldMap->getTeam(VSSRef::Color::BLUE) + worldMap->getTeam(VSSRef::Color::YELLOW);
    for (Player* p : allPlayers) {
        if (p->getPlayerId() != player->getPlayerId()) {
            minDistance = std::min(minDistance, Basic::getDistance(robotPos, p->getCoordinates()));
        }
    }

    float activationRadius = 0.35f; 
    float alpha = 0.0f;
    if (minDistance < activationRadius) {
        alpha = 1.0f - (minDistance / activationRadius);
        alpha = alpha * alpha; 
    }
    
    attractiveField.normalize();
    repulsiveField.normalize();
    
    QVector2D finalVector = attractiveField * (1.0f - alpha) + repulsiveField * alpha;
    
    return finalVector.normalized();
}

QVector2D UnivectorField::avoidObstacleField(const QVector2D& robotPos, Player *player, WorldMap *worldMap, const QVector2D& attractiveVector) {
    QVector2D resultingVector(0, 0);

    auto allPlayers = worldMap->getTeam(VSSRef::Color::BLUE) + worldMap->getTeam(VSSRef::Color::YELLOW);
    for (Player *obstaclePlayer : allPlayers) {
        if(obstaclePlayer->getPlayerId() != player->getPlayerId()) {
            float distance = Basic::getDistance(robotPos, obstaclePlayer->getCoordinates());
            if (distance < D_MIN_ROBOT * 4.0f) {
                QVector2D repulsiveVector = robotPos - obstaclePlayer->getCoordinates();
                resultingVector += repulsiveVector.normalized() / distance;
            }
        }
    }

    float wallRepulsionThreshold = 0.08f; 
    float distTop = worldMap->getMaxY() - robotPos.y();
    float distBottom = robotPos.y() - worldMap->getMinY();
    float distRight = worldMap->getMaxX() - robotPos.x();
    float distLeft = robotPos.x() - worldMap->getMinX();

    QVector2D perpendicularVector(0,0);
    if (distTop < wallRepulsionThreshold) perpendicularVector += QVector2D(0, -1);
    if (distBottom < wallRepulsionThreshold) perpendicularVector += QVector2D(0, 1);
    if (distRight < wallRepulsionThreshold) perpendicularVector += QVector2D(-1, 0);
    if (distLeft < wallRepulsionThreshold) perpendicularVector += QVector2D(1, 0);

    if (perpendicularVector.length() > 0) {
        perpendicularVector.normalize();
        
        QVector2D tangentVector(-perpendicularVector.y(), perpendicularVector.x());

        if (QVector2D::dotProduct(tangentVector, attractiveVector) < 0) {
            tangentVector = -tangentVector;
        }

        resultingVector += (perpendicularVector * 0.2f + tangentVector * 0.8f);
    }

    return resultingVector;
}


QVector2D UnivectorField::moveToGoalField(const QVector2D& p, const QVector2D& target, WorldMap* worldMap, Player* player) {
    QVector2D relativePos = p - target;
    
    VSSRef::Color opponentColor = (player->getPlayerColor() == VSSRef::BLUE) ? VSSRef::YELLOW : VSSRef::BLUE;
    QVector2D opponentGoal = worldMap->getOurGoalCenter(opponentColor);
    float attackAngle = atan2(opponentGoal.y() - target.y(), opponentGoal.x() - target.x());

    float angleToRotate = -attackAngle;
    QVector2D rotatedP(
        relativePos.x() * cos(angleToRotate) - relativePos.y() * sin(angleToRotate),
        relativePos.x() * sin(angleToRotate) + relativePos.y() * cos(angleToRotate)
    );

    float x = rotatedP.x();
    float y = rotatedP.y();

    QVector2D p_l(x, y - DE);
    QVector2D p_r(x, y + DE);
    
    float angle_cw = hyperbolicSpiral(p_r, QVector2D(0,0), DE, KR, true);
    QVector2D n_cw(cos(angle_cw), sin(angle_cw));

    float angle_ccw = hyperbolicSpiral(p_l, QVector2D(0,0), DE, KR, false);
    QVector2D n_ccw(cos(angle_ccw), sin(angle_ccw));
    
    QVector2D finalRotatedVector;
    if (y > -DE && y < DE) {
        float y_l = y + DE;
        float y_r = DE - y;
        finalRotatedVector = (y_l * n_ccw + y_r * n_cw) / (2 * DE);
    } else if (y <= -DE) {
        finalRotatedVector = n_cw;
    } else {
        finalRotatedVector = n_ccw;
    }

    float finalAngle = atan2(finalRotatedVector.y(), finalRotatedVector.x()) + attackAngle;
    
    return QVector2D(cos(finalAngle), sin(finalAngle));
}

float UnivectorField::hyperbolicSpiral(const QVector2D& p, const QVector2D& target, float de, float kr, bool clockwise) {
    QVector2D relativePos = p - target;
    float rho = relativePos.length();
    float theta = atan2(relativePos.y(), relativePos.x());
    float sign = clockwise ? 1.0f : -1.0f;

    if(rho > de) {
        return theta + sign * (M_PI / 2.0f) * (2.0f - (de + kr) / (rho + kr));
    } else {
        return theta + sign * (M_PI / 2.0f) * sqrt(rho / de);
    }
}