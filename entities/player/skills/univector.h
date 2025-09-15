#ifndef UNIVECTOR_H
#define UNIVECTOR_H

#include <QVector2D>

class Player;
class WorldMap;

class UnivectorField {

public:
    static QVector2D getDirection(Player *player, const QVector2D& target, WorldMap *worldMap);

private:
    static constexpr float DE = 0.08f;
    static constexpr float KR = 0.2f;
    static constexpr float D_MIN = 0.09f;
    static constexpr float D_MIN_ROBOT = 0.0003f;
    static constexpr float DELTA = 0.5515f; 
    static constexpr float K0 = 0.0448f;

    static constexpr float SIGMA_OPPONENT = 0.3f;
    static constexpr float SIGMA_TEAMMATE = 0.3f;
    static constexpr float SIGMA_WALLS = 0.5f;
    
    static constexpr float D_MIN_WALLS = 0.0064f;

    static float hyperbolicSpiral(const QVector2D& p, const QVector2D& target, float de, float kr, bool clockwise);
    static QVector2D moveToGoalField(const QVector2D& p, const QVector2D& target, WorldMap* worldMap, Player* player);
    static QVector2D avoidObstacleField(const QVector2D& robotPos, Player *player, WorldMap *worldMap, const QVector2D& attractiveVector);
};

#endif // UNIVECTOR_H