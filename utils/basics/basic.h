#ifndef BASIC_H
#define BASIC_H

#include <QVector2D>

class Basic 

{

public:

    static float getAngle(QVector2D currentPos, QVector2D &targetPos);

    static float getDistance(QVector2D currentPos, QVector2D &targetPos);

    static float smallestAngleDiff(float &currentAng , float &targetAng);

    static bool checkCanBeReversed(float &currentAng, float targetAng);

    static float normalizeAngle(const float &angle);
};


#endif // BASIC_H