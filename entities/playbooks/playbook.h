#ifndef PLAYBOOK_H
#define PLAYBOOK_H

#include "player/roles/role.h"
#include "worldmap/worldmap.h"
#include "actuator/actuator.h"

#include <vector>
#include <memory>
#include <map>

/**
 * @brief Base class for all playbooks in the hierarchical control system
 * 
 * Playbooks are responsible for grouping roles and assigning them to players. Each playbook represents a specific team strategy like offense, defense, etc.
 */
class Playbook 
{
public:
    Playbook(WorldMap *worldMap, VSSRef::Color teamColor) : _worldMap(worldMap), _teamColor(teamColor), _isActive(false) {}

    virtual ~Playbook() = default;

    virtual void executePlaybook(ActuatorClient *actuator);

    virtual bool shouldActivate() = 0;

    void activate();
    void deactivate();

    bool isActive() const { return _isActive; }
    
    virtual int getPriority() const = 0;

protected:
    virtual void initializeRoles() = 0;

    virtual void updatePlaybookState() {}

    void assignRole(int playerId, std::unique_ptr<Role> Role);

    WorldMap *_worldMap;
    VSSRef::Color _teamColor;
    std::map<int, std::unique_ptr<Role>> _roles; // Map of player IDs to roles
    bool _isActive;
 
};

#endif // PLAYBOOK_H