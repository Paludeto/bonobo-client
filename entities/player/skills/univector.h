#ifndef UNIVECTOR_H
#define UNIVECTOR_H

#include <QVector2D>

class Player;
class WorldMap;

class UnivectorField {

public:
    static QVector2D getDirection(Player *player, const QVector2D& target, WorldMap *worldMap);

private:
    static constexpr float DE = 0.05f;
    static constexpr float KR = 0.3212f;
    static constexpr float D_MIN_ROBOT = 0.0588f;
    static constexpr float DELTA = 0.5515f; 

    static float hyperbolicSpiral(const QVector2D& p, const QVector2D& target, float de, float kr, bool clockwise);
    static QVector2D moveToGoalField(const QVector2D& p, const QVector2D& target, WorldMap* worldMap, Player* player);
    static QVector2D avoidObstacleField(const QVector2D& robotPos, Player *player, WorldMap *worldMap, const QVector2D& attractiveVector);
};

#endif // UNIVECTOR_H