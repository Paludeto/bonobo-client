#include "RRTPathExecution.h"
#include "skill_rrt.h"  
#include "RRTCollision.h"
#include "RRTCore.h"
#include "RRTOptimization.h"
#include "player/player.h"
#include "basics/basic.h"
#include <cmath>
#include <limits>
#include <algorithm>

void RRTPathExecution::extractPath(RRT* rrt, int goalNodeIdx) {
    // Clear existing path
    rrt->_finalPath.clear();

    // Start from the goal node and trace back to the root
    int currentIdx = goalNodeIdx;
    
    while(currentIdx != -1) {
        // Add the current node to the path
        rrt->_finalPath.prepend(rrt->_treeNodes[currentIdx].position);
        
        // Move to the parent node
        currentIdx = rrt->_treeNodes[currentIdx].parent;
    }
    
    // Use our enhanced path simplification algorithm from RRTOptimization
    if(rrt->_finalPath.size() > 2) {
        rrt->_finalPath = RRTOptimization::simplifyPath(rrt, rrt->_finalPath);
    }
}

bool RRTPathExecution::executePathStep(RRT* rrt, ActuatorClient *actuator) {
    if (rrt->_currentPathIndex >= rrt->_finalPath.size()) {
        return true; // Path is completed
    }
    
    QVector2D currentWaypoint = rrt->_finalPath[rrt->_currentPathIndex];
    QVector2D robotPosition = rrt->_player->getCoordinates();
    
    // Check if the lookahead path is clear
    float robotSpeed = rrt->_player->getLinearSpeed();
    const float TIME_HORIZON = 0.3f; // 300 ms
    int lookaheadSteps = static_cast<int>((robotSpeed * TIME_HORIZON) / (RRT::DEFAULT_STEP_SIZE + 0.001f));
    lookaheadSteps = std::clamp(lookaheadSteps, 1, 10);
    if (!RRTCollision::isLookaheadClear(rrt, robotPosition, rrt->_currentPathIndex, lookaheadSteps)) {
        rrt->_needsReplanning = true;
        RRTCore::initializeTree(rrt);
        return false;
    }
    
    // If already reached the current waypoint, advance to the next
    if (Basic::getDistance(robotPosition, currentWaypoint) < rrt->_pathFollowingThreshold) {
        rrt->_currentPathIndex++;
        if (rrt->_currentPathIndex >= rrt->_finalPath.size()) {
            rrt->_pathCompleted = true;
            return true; // Path completed
        }
        currentWaypoint = rrt->_finalPath[rrt->_currentPathIndex];
    }
    
    // Improve path following with dynamic lookahead
    // For smoother movement, target a point farther along the path when possible
    int lookaheadIndex = rrt->_currentPathIndex;
    const float maxLookaheadDistance = 0.15f; // 15cm maximum lookahead
    float accumulatedDistance = 0.0f;
    
    // Look for a waypoint that's within our lookahead distance
    for (int i = rrt->_currentPathIndex; i < rrt->_finalPath.size() - 1; i++) {
        float segmentDistance = Basic::getDistance(rrt->_finalPath[i], rrt->_finalPath[i+1]);
        if (accumulatedDistance + segmentDistance > maxLookaheadDistance) {
            // Calculate interpolated position based on remaining distance
            float remainingDistance = maxLookaheadDistance - accumulatedDistance;
            float ratio = remainingDistance / segmentDistance;
            QVector2D direction = rrt->_finalPath[i+1] - rrt->_finalPath[i];
            QVector2D lookaheadPoint = rrt->_finalPath[i] + direction * ratio;
            
            // Check if direct path to lookahead point is clear
            if (RRTCollision::isObstacleFree(rrt, robotPosition, lookaheadPoint)) {
                rrt->_player->goTo(lookaheadPoint, actuator);
                return false;
            }
            break;
        }
        accumulatedDistance += segmentDistance;
        lookaheadIndex = i + 1;
    }
    
    // If lookahead optimization failed, just go to the current waypoint
    rrt->_player->goTo(currentWaypoint, actuator);
    return false;
}

bool RRTPathExecution::executePredictiveManeuver(RRT* rrt, ActuatorClient *actuator) {
    if (!rrt || !rrt->_isPerformingPredictiveManeuver || !rrt->_predictiveManeuver) {
        std::cout << "RRT: Error - Invalid predictive maneuver state\n";
        return true; // Return true to exit maneuver mode
    }
    
    try {
        // Run the predictive maneuver skill
        rrt->_predictiveManeuver->runSkill(actuator);
        
        // Verificação de segurança antes de chamar isComplete
        if (rrt->_predictiveManeuver) {
            bool complete = rrt->_predictiveManeuver->isComplete();
            return complete;
        }
    } catch (const std::exception& e) {
        std::cout << "RRT: Exception in predictive maneuver: " << e.what() << std::endl;
        return true; // Exit maneuver mode on exception
    } catch (...) {
        std::cout << "RRT: Unknown exception in predictive maneuver" << std::endl;
        return true; // Exit maneuver mode on exception
    }
    
    return false;
}