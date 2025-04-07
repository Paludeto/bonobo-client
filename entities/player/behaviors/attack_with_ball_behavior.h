#ifndef ATTACK_WITH_BALL_BEHAVIOR_H
#define ATTACK_WITH_BALL_BEHAVIOR_H

#include "player/behaviors/behavior.h"
#include "worldmap/worldmap.h"
#include "player/player.h"
#include "actuator/actuator.h"

/**
 * @brief Simplified behavior for attacking with the ball
 * 
 * When the player has the ball, drives it toward the opponent's goal.
 */
class AttackWithBallBehavior : public Behavior 
{
public:
    /**
     * @brief Constructor for AttackWithBallBehavior
     */
    AttackWithBallBehavior(Player *player, WorldMap *worldMap, float ownGoalX, float ownGoalY, float opponentGoalX, float opponentGoalY);
    
    ~AttackWithBallBehavior() = default;

    /**
     * @brief Execute the behavior
     */
    void execute(ActuatorClient *actuator) override;

    /**
     * @brief Check if this behavior should be activated
     */
    bool shouldActivate() override;

    /**
     * @brief Check if this behavior should remain active
     */
    bool shouldKeepActive() override;

    /**
     * @brief Get the priority of this behavior
     */
    int getPriority() const override;

    enum State {STATE_ATTACK, POSITIONING_STATE, TAKE_BALL_STATE};
private:
    // Field positions
    float _ownGoalX;
    float _ownGoalY;
    float _opponentGoalX;
    float _opponentGoalY;
    State _state;
    
    // Ball possession tracking
    bool _hadBallPossession;
    
    // Constants
    static constexpr float BALL_POSSESSION_DIST = 0.09f;  // Distance for ball possession
    
    /**
     * @brief Check if player has the ball
     */
    bool hasBallPossession() const;
    QVector2D calculateBestPosition();
};

#endif // ATTACK_WITH_BALL_BEHAVIOR_H