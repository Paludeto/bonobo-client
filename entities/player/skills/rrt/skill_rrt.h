#ifndef SKILL_RRT_H
#define SKILL_RRT_H

#include "player/skills/skill_manager.h"
#include "basics/basic.h"
#include "worldmap/worldmap.h"
#include "player/skills/skill_escape.h"

#include <QVector2D>
#include <QList>
#include <random>
#include <vector>

// Structure to represent a node in RRT tree
struct RRTNode {
    QVector2D position; // Position in the field
    int parent;         // Index of the parent node in the tree 
    float costToStart;  // Cost (distance) from start to this node
    RRTNode(QVector2D pos, int p = -1, float cost = 0.0f) : position(pos), parent(p), costToStart(cost) {}
};

// Forward declarations of friend classes
class RRTCore;
class RRTCollision;
class RRTPathExecution;
class RRTOptimization;

/**
 * @brief A pathfinding skill that uses RRT (Rapidly-exploring Random Tree) algorithm 
 * 
 * This class implements the RRT/RRT* algorithm for pathfinding in the VSS environment.
 * It uses a modular approach to separate tree building, path execution, collision detection,
 * and path optimization.
 */
class RRT : public SkillManager 
{
public:
    // Constants for RRT parameters 
    static constexpr float DEFAULT_STEP_SIZE = 0.05f;      // Default step size for tree expansion
    static constexpr float DEFAULT_GOAL_BIAS = 0.5f;       // Probability of sampling the goal directly
    static constexpr int DEFAULT_MAX_ITERATIONS = 5000;    // Maximum iterations for tree growth
    static constexpr float DEFAULT_FIELD_WIDTH = 1.5f;     // Default field width in meters 
    static constexpr float DEFAULT_FIELD_HEIGHT = 1.3f;    // Default field height in meters
    static constexpr int MAX_ITERATIONS_PER_FRAME = 60;    // Maximum iterations per frame to spread computation 
    const float MOVEMENT_THRESHOLD = 0.005f;               // 5mm per frame is minimum movement

    /**
     * @brief Construct a new RRT object 
     * 
     * @param player Pointer to the player using skill
     * @param targetPosition Target position to reach
     * @param worldMap Pointer to the worldMap for obstacle detection
     * @param robotRadius Radius of the robot for collision detection 
     */
    RRT(Player *player, QVector2D &targetPosition, WorldMap *worldMap, float robotRadius, int stuckCounter);

    /**
     * @brief Implementation of the runSkill method from SkillManager
     * Performs incremental path finding and follows the path if found
     * 
     * @param actuator Actuator client for sending commands to the robot
     */
    void runSkill(ActuatorClient *actuator) override;

    /**
     * @brief Check if a path was found
     * 
     * @return true if a path was found, false otherwise
     */
    bool isPathFound() const { return _pathFound; }

    /**
     * @brief Get the target position
     * 
     * @return Target position vector
     */
    const QVector2D& getTargetPosition() const { return _targetPosition; }

    int getStuckCounter() const { return _stuckCounter; }

private:
    // Make classes friends to access private members
    friend class RRTCore;
    friend class RRTCollision;
    friend class RRTPathExecution;
    friend class RRTOptimization;

    // Member variables 
    QVector2D _targetPosition;                        // Target position to reach
    WorldMap *_worldMap;                              // Reference to world state
    std::vector<RRTNode> _treeNodes;                  // Nodes of the RRT tree
    QList<QVector2D> _finalPath;                      // Final path from start to goal
    int _currentPathIndex;                            // Current index in the path being followed
    int _currentIteration;                            // Current iteration count for tracking progress

    // Parameters
    int _maxIterations;                               // Maximum number of iterations  
    float _robotRadius;                               // Radius of the robot for collision checking  
    float _pathFollowingThreshold;                    // Distance threshold for path following

    // State variables 
    bool _pathFound;                                  // Flag indicating if a path is found
    bool _initialized;                                // Flag indicating if tree is initialized
    bool _pathCompleted;                              // Flag indicating if the current path was completed
    bool _needsReplanning;                            // Flag indicating if path needs to be recalculated

    // Random number generation
    std::mt19937 _rng;                                // Random number generator
    std::uniform_real_distribution<float> _unitDist;  // Distribution for unit interval [0,1]

    // Predictive maneuver
    bool _isPerformingPredictiveManeuver;             // Flag indicating if performing predictive maneuver
    std::unique_ptr<PredictiveManeuver> _predictiveManeuver; // Predictive maneuver object
    int _stuckCounter; 
};

#endif // SKILL_RRT_H