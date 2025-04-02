#ifndef DEFENSIVE_PLAYBOOK_H
#define DEFENSIVE_PLAYBOOK_H

#include "playbook.h"

/**
 * @brief Defensive playbook for team strategy
 * 
 * This playbook focuses on defensive play, assigning
 * one goalkeeper, one defender, and one attacker.
 */
class DefensivePlaybook : public Playbook
{
public:
    DefensivePlaybook(WorldMap *worldMap, VSSRef::Color teamColor, float ownGoalX, float ownGoalY, float opponentGoalX, float opponentGoalY);

    /**
     * @brief Check if this playbook should be activated
     * 
     * Activates when the ball is in our half or an opponent has the ball
     * 
     * @return true if conditions for activation are met
     */
    bool shouldActivate() override;

    /**
     * @brief Get the priority of this playbook
     * 
     * @return High priority when in defensive situations
     */
    int getPriority() const override;

protected:
    /**
     * @brief Initialize and assign roles to players
     * 
     * Creates goalkeeper, defender, and attacker roles
     */
    void initializeRoles() override;

    /**
     * @brief Update playbook-specific state
     * 
     * Called before executing roles
     */
    void updatePlaybookState() override;

private:
    
    bool isOpponentWithBall() const;

    float _ownGoalX;
    float _ownGoalY;
    float _opponentGoalX;
    float _opponentGoalY;
    
    // Constants
    static constexpr float BALL_CONTROL_THRESHOLD = 0.1f;
};

#endif // DEFENSIVE_PLAYBOOK_H