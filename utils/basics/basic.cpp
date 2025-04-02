#include "basic.h"
#include "worldmap/worldmap.h"

#include <math.h>


float Basic::getAngle(QVector2D currentPos, QVector2D targetPos) {

    return atan2(targetPos.y() - currentPos.y(), targetPos.x() - currentPos.x());

}

float Basic::getDistance(const QVector2D currentPos, const QVector2D &targetPos) {

    return sqrt(pow(targetPos.x() - currentPos.x(), 2) + pow(targetPos.y() - currentPos.y(), 2));

}

float Basic::smallestAngleDiff(float &currentPos, float &targetPos) {
    float a = fmod(currentPos + 2*M_PI, 2*M_PI) - fmod(targetPos + 2*M_PI, 2*M_PI);
    
    if(a > M_PI) {
        a = a - 2 * M_PI;
    } 
    else if(a < -M_PI) {
        a = a + 2 * M_PI;
    }
    
    return a;
}


bool Basic::checkCanBeReversed(float &currentAng, float targetAng) {
    float angDiff = Basic::smallestAngleDiff(currentAng, targetAng);

    return (fabs(angDiff) > M_PI/2.0 + M_PI/20);  
}

float Basic::normalizeAngle(const float &angle) {
    float angleRet = fmod(angle + M_PI, 2.0 * M_PI);
    angleRet = (angleRet < 0.0) ? angleRet + M_PI : angleRet - M_PI;

    return angleRet;
}

QVector2D Basic::getBallImpactPosition(float incomingAngle, float posX, WorldMap *wm, VSSRef::Color color) {
    float angle  = incomingAngle; 

    if(wm->isOurSideLeft(color)) {
        angle = M_PI - angle;
    }

    if(angle == M_2_PI) {
        angle = 0.0f;
    }

    if(angle >= M_PI) {
        angle += (angle > 0 ? -M_2_PI : M_2_PI);
    }

    if(fabs(angle) >= M_PI/2) {
        return QVector2D(0.0, 0.0);
    }

    float x = fabs(posX - wm->getBallPosition().x());
    float tan = std::tan(angle);
    float y = tan * x;
    
    float y_impact = wm->getBallPosition().y() + y;

    return QVector2D(posX, y_impact);
}

QVector2D Basic::calculateItermediatePoint(QVector2D near, QVector2D far, float distance, float beta) {
    float alpha = std::atan2(far.y() - near.y(), far.x() - near.x());
    float gamma = alpha + beta;

    return QVector2D(near.x() + distance * std::cos(gamma), near.y() + distance * std::sin(gamma));
}