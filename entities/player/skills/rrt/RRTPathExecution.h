#ifndef RRT_PATH_EXECUTION_H
#define RRT_PATH_EXECUTION_H

#include <QVector2D>
#include <vector>

class RRT;
class ActuatorClient;

/**
 * @brief Path execution functionality for RRT
 * 
 * Contains methods for extracting and following paths.
 */
class RRTPathExecution {
public:
    /**
     * @brief Extract the final path from the tree
     * 
     * @param rrt Reference to the main RRT object
     * @param goalNodeIdx Index of the goal node
     */
    static void extractPath(RRT* rrt, int goalNodeIdx);
    
    /**
     * @brief Execute a step of path following
     * 
     * @param rrt Reference to the main RRT object
     * @param actuator Actuator client for sending commands
     * @return true if path is completed, false otherwise
     */
    static bool executePathStep(RRT* rrt, ActuatorClient *actuator);
    
    /**
     * @brief Execute predictive maneuver
     * 
     * @param rrt Reference to the main RRT object
     * @param actuator Actuator client for sending commands
     * @return true if maneuver is completed
     */
    static bool executePredictiveManeuver(RRT* rrt, ActuatorClient *actuator);
};

#endif // RRT_PATH_EXECUTION_H