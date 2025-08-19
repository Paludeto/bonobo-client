#ifndef ATTACKER_ROLE_H
#define ATTACKER_ROLE_H

#include "player/roles/role.h"
#include "worldmap/worldmap.h"
#include "player/player.h"
#include "actuator/actuator.h"

/**
 * @brief Role for the attacker player
 * 
 * The attacker role focuses on scoring goals. It switches between offensive behavior
 * when in possession of the ball and defensive behavior when the opponent has the ball.
 */
class AttackerRole : public Role 
{
public:
    /**
     * @brief Constructor for AttackerRole
     * 
     * @param player Pointer to the player assigned this role
     * @param worldMap Pointer to the world map for field information
     * @param ownGoalX X coordinate of our goal
     * @param ownGoalY Y coordinate of our goal
     * @param opponentGoalX X coordinate of opponent's goal
     * @param opponentGoalY Y coordinate of opponent's goal
     */
    AttackerRole(Player *player, WorldMap *worldMap, float ownGoalX, float ownGoalY, 
                float opponentGoalX, float opponentGoalY);
    ~AttackerRole() = default;

protected:
    /**
     * @brief Initialize attacker-specific behaviors
     * 
     * Creates and adds behaviors that the attacker needs to perform its role.
     */
    void initializeBehaviors() override;

    /**
     * @brief Update role-specific state
     * 
     * Updates any state information specific to the attacker role.
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

#endif // ATTACKER_ROLE_H