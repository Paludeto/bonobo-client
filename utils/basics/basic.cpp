#include "basic.h"


float Basic::getAngle(QVector2D currentPos, QVector2D &targetPos) {

    return atan2(targetPos.y() - currentPos.y(), targetPos.x() - currentPos.x());

}

float Basic::getDistance(QVector2D currentPos, QVector2D &targetPos) {

    return sqrt(pow(targetPos.x() - currentPos.x(), 2) + pow(targetPos.y() - currentPos.y(), 2));

}