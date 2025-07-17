#ifndef ROLE_H
#define ROLE_H

#include "player/player.h"
#include "worldmap/worldmap.h"
#include "actuator/actuator.h"
#include "player/behaviors/behavior.h"
#include "player/skills/skill_wall_spin.h"

#include <memory>
#include <vector>
#include <string>
#include <QElapsedTimer> 

/**
 * @brief Base class for all roles in the hierachical control system
 * 
 * Roles represent a player's function in the team. Each role may contains multiple behaviors and is responsible for selecting the most appropriate behabior
 * based on the current game state and priorities.
 */
class Role 
{
public:
    Role(Player *player, WorldMap *worldMap) : _player(player), _worldMap(worldMap), _activeBehaviorIndex(-1), _isStuckOnWall(false) { _lastPosition = _player->getCoordinates(); }

    virtual ~Role() = default;

    /**
     * Evaluates all behaviors and select the highest priority one that should be activated, and then executes it.
     */
    virtual void executeRole(ActuatorClient *actuator);

    Behavior *getActiveBehavior();

    const std::vector<std::unique_ptr<Behavior>> &getBehaviors() const {
        return _behaviors;
    }

protected:
    virtual void initializeBehaviors() = 0;
    virtual void updateRoleState() {}
    virtual int selectBehavior(); // Return Index or -1 if none

    Player *_player;
    WorldMap *_worldMap;
    std::vector<std::unique_ptr<Behavior>> _behaviors; // Vector of smart pointers of behaviors
    int _activeBehaviorIndex;

    bool _isStuckOnWall;
    QElapsedTimer _stuckTimer;
    QVector2D _lastPosition;
    std::unique_ptr<WallSpinSkill> _wallSpinSkill;
};


#endif // ROLE_H