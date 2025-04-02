#ifndef COACH_H
#define COACH_H

#include "worldmap/worldmap.h"
#include "actuator/actuator.h"
#include "strategy/strategy.h"

using VSSRef::Color;

/**
 * @brief Top-level control entity for the team
 * 
 * The Coach is responsible for managing the overall team strategy and
 * coordinating the players. It is the entry point for the control hierarchy.
 */
class Coach
{
public:
    Coach(WorldMap *wm, ActuatorClient *actuator, Color color);
    void runCoach();
    void setTeam(Color color);
    Strategy* getStrategy() const {
        return _strategy.get();
    }

private:
    ActuatorClient *_actuator;             
    WorldMap *_wm;                         
    QList<Player *> _ourTeam;              
    Color _ourColor;                       
    float _ownGoalX;                       
    float _ownGoalY;                       
    float _opponentGoalX;                  
    float _opponentGoalY;                  
    std::unique_ptr<Strategy> _strategy;   
};

#endif // COACH_H