#ifndef DEFENDER_ROLE_H
#define DEFENDER_ROLE_H

#include "player/roles/role.h"
#include "worldmap/worldmap.h"
#include "player/player.h"
#include "actuator/actuator.h"

/**
 * @brief Role for a defender player
 * 
 * The defender role focuses on blocking opponent attacks and protecting
 * our goal area. It manages behaviors related to defensive positioning
 * and ball recovery.
 */
class DefenderRole : public Role 
{
public:
    /**
     * @brief Constructor for DefenderRole
     * 
     * @param player Pointer to the player assigned this role
     * @param worldMap Pointer to the world map for field information
     * @param ownGoalX X coordinate of our goal
     * @param ownGoalY Y coordinate of our goal
     * @param opponentGoalX X coordinate of opponent's goal
     * @param opponentGoalY Y coordinate of opponent's goal
     */
    DefenderRole(Player *player, WorldMap *worldMap, float ownGoalX, float ownGoalY, 
                float opponentGoalX, float opponentGoalY);
    ~DefenderRole() = default;

protected:
    /**
     * @brief Initialize defender-specific behaviors
     * 
     * Creates and adds behaviors that the defender needs to perform its role.
     */
    void initializeBehaviors() override;

    /**
     * @brief Update role-specific state
     * 
     * Updates any state information specific to the defender role.
     */
    void updateRoleState() override;

private:
    // Goal positions
    float _ownGoalX;
    float _ownGoalY;
    float _opponentGoalX;
    float _opponentGoalY;
    
    // Game state tracking
    bool _wasControllingBall;
    int _behaviorTransitionCounter;
};

#endif // DEFENDER_ROLE_H