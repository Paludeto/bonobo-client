#ifndef STRATEGY_H
#define STRATEGY_H

#include "playbooks/playbook.h"
#include "worldmap/worldmap.h"
#include "actuator/actuator.h"

#include <vector>
#include <memory>

/**
 * @brief Top-level control system for team strategy
 * 
 * Strategy is the highest level in the hierarchy. It manages multiple playbooks and selects the most appropriate one based on the game state.
 */
class Strategy 
{
public:
    Strategy(WorldMap *worldMap, ActuatorClient *actuator, VSSRef::Color teamColor, float ownGoalX, float ownGoalY, float opponentGoalX, float opponentGoalY);

    ~Strategy() = default;

    void executeStrategy();

private:
    void initializePlaybooks();

    Playbook *selectPlaybook();

    void updateGameState();

    WorldMap *_worldMap;
    ActuatorClient *_actuator;
    VSSRef::Color _teamColor;
    float _ownGoalX;
    float _ownGoalY;
    float _opponentGoalX;
    float _opponentGoalY;

    std::vector<std::unique_ptr<Playbook>> _playbooks;
    Playbook *_activePlaybook;
    int _gameState;
};

#endif // STRATEGY_H