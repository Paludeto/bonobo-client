#ifndef COACH_H
#define COACH_H

#include "worldmap/worldmap.h"
#include "actuator/actuator.h"

using VSSRef::Color;

class Coach
{
public:
    Coach(WorldMap *wm, ActuatorClient *actuator, Color color) 
        : _wm(wm), _actuator(actuator), _ourColor(color)
    {
        _actuator->setTeamColor(color);
    }

    void runCoach();
    void setTeam(Color color);

private:
    ActuatorClient *_actuator;
    WorldMap *_wm;
    QList<Player *> _ourTeam;
    Color _ourColor;
};

#endif // COACH_H