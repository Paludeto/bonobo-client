#include "basic.h"

#include <math.h>


float Basic::getAngle(QVector2D currentPos, QVector2D &targetPos) {

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