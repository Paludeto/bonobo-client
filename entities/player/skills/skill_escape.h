#ifndef SKILL_ESCAPE_H
#define SKILL_ESCAPE_H

#include "skill_manager.h"
#include "basics/basic.h"
#include "worldmap/worldmap.h"
#include <QVector2D>

/**
 * @brief A skill for predictive evasive maneuvers when the robot is stuck
 * 
 * This skill performs intelligent evasive maneuvers when the robot is stuck,
 * predicting ball movement and positioning the robot for interception.
 */
class PredictiveManeuver : public SkillManager {
public:
    /**
     * @brief Constructor for the PredictiveManeuver class
     * 
     * @param player Pointer to the player executing the maneuver
     * @param worldMap Pointer to the world map for environment information
     * @param lastTarget The target the robot was attempting to reach
     * @param predictionTime How far into the future to predict (seconds)
     */
    PredictiveManeuver(Player *player, WorldMap *worldMap);
    
    /**
     * @brief Runs the predictive maneuver skill
     * 
     * @param actuator Actuator client for sending commands
     */
    void runSkill(ActuatorClient *actuator) override;
    
    /**
     * @brief Check if the maneuver is completed
     * 
     * @return True if the maneuver is complete
     */
    bool isComplete() { return _maneuverComplete; }
    
private:
    WorldMap* _worldMap;                  // Reference to world state
    bool _maneuverComplete;               // Flag indicating completion
    float _maneuverTimer;                 // Timer for the current maneuver step
};

#endif // SKILL_ESCAPE_H