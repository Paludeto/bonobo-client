#ifndef BASIC_H
#define BASIC_H

#include "vssref_common.pb.h"
#include <QVector2D>

class WorldMap;

class Basic 

{

public:

    static float getAngle(QVector2D currentPos, QVector2D targetPos);

    static float getDistance(const QVector2D currentPos, const QVector2D &targetPos);

    static float smallestAngleDiff(float &currentAng , float &targetAng);

    static bool checkCanBeReversed(float &currentAng, float targetAng);

    static float normalizeAngle(const float &angle);

    static QVector2D getBallImpactPosition(float incomingAngle, float posX, WorldMap *wm, VSSRef::Color color); 

    static QVector2D calculateItermediatePoint(QVector2D near, QVector2D far, float distance, float beta);
};


#endif // BASIC_H