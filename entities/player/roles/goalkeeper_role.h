#ifndef GOALKEEPER_ROLE_H
#define GOALKEEPER_ROLE_H

#include "role.h"
#include "worldmap/worldmap.h"
#include "player/player.h"
#include "actuator/actuator.h"

/**
 * @brief Role for the goalkeeper player
 * 
 * The goalkeeper role focuses on protecting our goal. It manages behaviors
 * related to goal defense and ball clearance when necessary.
 */
class GoalkeeperRole : public Role 
{
public:
    /**
     * @brief Constructor for GoalkeeperRole
     * 
     * @param player Pointer to the player assigned this role
     * @param worldMap Pointer to the world map for field/object information
     * @param ownGoalX X coordinate of our goal
     * @param ownGoalY Y coordinate of our goal
     */
    GoalkeeperRole(Player *player, WorldMap *worldMap, float ownGoalX, float ownGoalY);
    ~GoalkeeperRole() = default;

protected:
    /**
     * @brief Initialize goalkeeper-specific behaviors
     * 
     * Creates and adds behaviors that the goalkeeper needs to perform its role.
     */
    void initializeBehaviors() override;

    /**
     * @brief Update role-specific state
     * 
     * Updates any state information specific to the goalkeeper role.
     */
    void updateRoleState() override;

private:
    // Position of our goal
    float _ownGoalX;
    float _ownGoalY;
};

#endif // GOALKEEPER_ROLE_H