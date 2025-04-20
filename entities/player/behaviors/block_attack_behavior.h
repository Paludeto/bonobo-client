#ifndef BLOCK_ATTACK_BEHAVIOR_H
#define BLOCK_ATTACK_BEHAVIOR_H

#include "player/behaviors/behavior.h"
#include "worldmap/worldmap.h"
#include "player/player.h"
#include "actuator/actuator.h"

/**
 * @brief Simplified behavior for blocking opponent attacks
 * 
 * This behavior activates when the ball is in our half or an opponent has it.
 * The player will position to block the ball or take possession.
 */
class BlockAttackBehavior : public Behavior 
{
public:
    /**
     * @brief Constructor for BlockAttackBehavior
     */
    BlockAttackBehavior(Player *player, WorldMap *worldMap, 
                      float ownGoalX, float ownGoalY, 
                      float opponentGoalX, float opponentGoalY);
    
    ~BlockAttackBehavior() = default;

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

    enum State {POSITIONING_STATE, TAKE_BALL_STATE};

private:
    // Field positions
    float _ownGoalX;
    float _ownGoalY;
    float _opponentGoalX;
    float _opponentGoalY;
    State _state;
    
    // Last ball position
    QVector2D _lastBallPos;
    
    // Constants
    static constexpr float BALL_POSSESSION_DIST = 0.1f;  // Distance for ball possession
    
    /**
     * @brief Check if an opponent has the ball
     */
    bool isOpponentWithBall() const;
    
    /**
     * @brief Check if we're the closest teammate to the ball
     */
    bool isClosestToBall() const;
    
    /**
     * @brief Calculate position to intercept ball
     */
    QVector2D calculateInterceptPosition() const;

    QVector2D calculateBestPosition();
};

#endif // BLOCK_ATTACK_BEHAVIOR_H