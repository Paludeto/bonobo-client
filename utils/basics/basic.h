#ifndef BASIC_H
#define BASIC_H

#include "vssref_common.pb.h"
#include <QVector2D>
#include <vector>

class WorldMap;

struct Triangle {
    QVector2D a, b, c;

    Triangle(QVector2D a, QVector2D b, QVector2D c) : a(a), b(b), c(c) {}


    bool containsVertex(const QVector2D v) const {
        return a == v || b == v || c == v;
    }

    bool operator == (const Triangle& other) const {
        return a == other.a && b == other.b && c == other.c;
    }
};

struct Edge {
    QVector2D p1, p2;

    bool operator == (const Edge other) const {
        return (p1 == other.p1 && p2 == other.p2) ||
               (p1 == other.p2 && p2 == other.p1);
    }
};

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

    static bool pointInCircumcicle(const QVector2D p, const Triangle tri);

    static std::vector<std::vector<QVector2D>> triangularization(std::vector<QVector2D> points);

    static QVector2D getCircumcenter(std::vector<QVector2D> t, WorldMap* worldMap);
};


#endif // BASIC_H