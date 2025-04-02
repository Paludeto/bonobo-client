#ifndef DEFEND_GOAL_BEHAVIOR_H
#define DEFEND_GOAL_BEHAVIOR_H

#include "player/behaviors/behavior.h"
#include "worldmap/worldmap.h"
#include "player/player.h"
#include "actuator/actuator.h"

/**
 * @brief Behavior for goalkeeper to defend the goal
 * 
 * This behavior makes the goalkeeper:
 * 1. Position along the goal line to intercept the ball
 * 2. Follow the ball's Y position for normal positioning
 * 3. Perform takeout when the ball is close to our goal area
 */
class DefendGoalBehavior : public Behavior 
{
public:
    DefendGoalBehavior(Player *player, WorldMap *worldMap, float ownGoalX, float ownGoalY);
    ~DefendGoalBehavior() = default;

    /**
     * @brief Execute goalkeeper behavior logic
     * 
     * @param actuator Actuator client for sending commands
     */
    void execute(ActuatorClient *actuator) override;

    /**
     * @brief Check if this behavior should be activated
     * 
     * For goalkeeper, this behavior has high priority and activates
     * when the ball is in our half or approaching our goal
     * 
     * @return true if conditions for activation are met
     */
    bool shouldActivate() override;

    /**
     * @brief Check if this behavior should remain active
     * 
     * @return true if the behavior should continue
     */
    bool shouldKeepActive() override;

    /**
     * @brief Get the priority of this behavior
     * 
     * @return High priority as defending goal is critical
     */
    int getPriority() const override;

    /**
     * @brief Set the distance from goal line for positioning
     * 
     * @param distGoal Distance from goal line (in meters)
     */
    void setDistGoal(float distGoal = 0.01f) { _distGoal = distGoal; }

private:
    // States
    enum State {
        STATE_GOTO,          // Normal positioning on goal line
        STATE_SHORTTAKEOUT   // Takeout when ball is in our area
    };

    // Current behavior state
    State _state;

    // Goal position
    float _ownGoalX;
    float _ownGoalY;
    
    // Parameters
    float _distGoal;

    /**
     * @brief Calculate the position for following the ball
     * 
     * @return QVector2D position along goal line tracking the ball's Y position
     */
    QVector2D getFollowBallPos();

    /**
     * @brief Check if ball is approaching our goal
     * 
     * @return true if ball is moving toward our goal
     */
    bool isBallComingToGoal(float postsFactor);

    /**
     * @brief Check if a point is inside our goal area
     * 
     * @param point Position to check
     * @param factor Area multiplier (for hysteresis)
     * @return true if the point is inside our area
     */
    bool isInsideOurArea(const QVector2D& point, float factor);

    // Constants
    static constexpr float BALL_MINVELOCITY = 0.05f;
    static constexpr float TAKEOUT_FACTOR_IN = 1.15f; 
    static constexpr float TAKEOUT_FACTOR_OUT = 1.30f;
    static constexpr float POSTSFACTOR = 1.75f;
};

#endif // DEFEND_GOAL_BEHAVIOR_H