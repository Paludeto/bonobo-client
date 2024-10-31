#ifndef COACH_H
#define COACH_H

#include "worldmap/worldmap.h"

using VSSRef::Color;

class Coach

{

public:
    
    Coach(WorldMap *wm, ActuatorClient *actuator) : _wm(wm), _actuator(actuator)
    {}

    void runCoach(Color color);

private:

    ActuatorClient *_actuator;
    WorldMap *_wm;
    QList<Player *> _ourTeam;

};


#endif