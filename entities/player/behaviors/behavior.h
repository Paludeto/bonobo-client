#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include "worldmap/worldmap.h"
#include "player/player.h"
#include "actuator/actuator.h"

#include <memory>

/**
 * @brief Base class for all behaviors in the hierarchical control system
 * 
 * Behaviors represent reactive components that evaluate field conditons and execute appropriate skills. 
 * They are the bridge between high-level roles and low-level skills, providing situational awereness and 
 * decision making.
 */

class Behavior 
{
public:
    Behavior(Player *player, WorldMap *worldMap) : _player(player), _worldMap(worldMap), _isActive(false) {}

    virtual ~Behavior() = default;

    /**
     * @brief Execute the behavior logic 
     * 
     * This method should contain the core decision-making of the behavior, evaluating conditions and executing appropriate skills.
     */
    virtual void execute(ActuatorClient *actuator) = 0;

    virtual bool shouldActivate() = 0;

    virtual bool shouldKeepActive() = 0;

    void activate() {
        _isActive = true;
    }

    void deactivate() {
        _isActive = false;
    }

    bool isActive() const {
        return _isActive;
    }

    /**
     * @brief Get the priority level of this behavior
     * 
     * Higher priority behaviors take precedence when multiple behaviors could be activated in a role.
     * 
     * @return Priority value
     */
    virtual int getPriority() const = 0;

protected:
    Player *_player;                 
    WorldMap *_worldMap;
    bool _isActive;
};

#endif // BEHAVIOR_H 