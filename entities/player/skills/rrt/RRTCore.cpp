#include "RRTCore.h"
#include "skill_rrt.h"  
#include "RRTCollision.h"
#include "RRTPathExecution.h"
#include "RRTOptimization.h"
#include "player/player.h"
#include "basics/basic.h"
#include <cmath>
#include <limits>
#include <algorithm>

void RRTCore::initializeTree(RRT* rrt) {
    // Clear existing tree
    rrt->_treeNodes.clear();
    rrt->_finalPath.clear();

    // Add the start position (current robot position) as the first node
    QVector2D startPos = rrt->_player->getCoordinates();
    addNode(rrt, startPos, -1, 0.0f);
}

QVector2D RRTCore::sampleRandomPoint(RRT* rrt) {
    // With a certain probability, sample directly toward the goal (goal bias)
    if(rrt->_unitDist(rrt->_rng) < RRT::DEFAULT_GOAL_BIAS) {
        return rrt->_targetPosition;
    }

    // Otherwise sample a random point in the field
    float x = rrt->_unitDist(rrt->_rng) * RRT::DEFAULT_FIELD_WIDTH - RRT::DEFAULT_FIELD_WIDTH/2.0f; // Assuming the field is centered at origin
    float y = rrt->_unitDist(rrt->_rng) * RRT::DEFAULT_FIELD_HEIGHT - RRT::DEFAULT_FIELD_HEIGHT/2.0f; 

    return QVector2D(x, y);
}

int RRTCore::findNearestNode(RRT* rrt, const QVector2D& point) {
    int nearestIdx = -1;
    float minDist = std::numeric_limits<float>::max();

    for(size_t i = 0; i < rrt->_treeNodes.size(); i++) {
        float dist = Basic::getDistance(rrt->_treeNodes[i].position, point);
        if(dist < minDist) {
            minDist = dist;
            nearestIdx = static_cast<int>(i);
        }
    }

    return nearestIdx;
}

QVector2D RRTCore::steer(RRT* rrt, const QVector2D& fromPoint, const QVector2D& toPoint) {
    QVector2D direction = toPoint - fromPoint;
    float distance = direction.length();

    // If points are too close, return the source point
    if(distance < 0.001f) {
        return fromPoint;
    }

    // If distance is less than step size, return the destination point 
    if(distance <= RRT::DEFAULT_STEP_SIZE) {
        return toPoint;
    }

    // Otherwise, take a step in the direction of the destination point
    direction.normalize();
    return fromPoint + direction * RRT::DEFAULT_STEP_SIZE;
}

int RRTCore::addNode(RRT* rrt, const QVector2D& position, int parent, float cost) {
    RRTNode node(position, parent, cost);
    rrt->_treeNodes.push_back(node);
    return static_cast<int>(rrt->_treeNodes.size() - 1); // return index
}

bool RRTCore::iterateRRT(RRT* rrt, int iterations) {
    for(int i = 0; i < iterations && rrt->_currentIteration < rrt->_maxIterations; i++, rrt->_currentIteration++) {
        // Sample a random point 
        QVector2D randomPoint = sampleRandomPoint(rrt);

        // Find the nearest node in the tree
        int nearestIdx = findNearestNode(rrt, randomPoint);
        if(nearestIdx < 0) continue; // empty

        // Steer toward the random point 
        QVector2D newPoint = steer(rrt, rrt->_treeNodes[nearestIdx].position, randomPoint);

        // Check if the path is obstacle-free
        if(RRTCollision::isObstacleFree(rrt, rrt->_treeNodes[nearestIdx].position, newPoint)) {
            // Calculate the cost to the new node
            float cost = rrt->_treeNodes[nearestIdx].costToStart + 
                         Basic::getDistance(rrt->_treeNodes[nearestIdx].position, newPoint);

            // Add the new node to the tree
            int newNodeIdx = addNode(rrt, newPoint, nearestIdx, cost);

            // RRT* improvement: rewire the tree
            RRTOptimization::rewireTree(rrt, newNodeIdx);

            // Check if we've reached the goal
            if(Basic::getDistance(newPoint, rrt->_targetPosition) < RRT::DEFAULT_STEP_SIZE) {
                // Add the goal node as a child of the new node
                float goalCost = cost + Basic::getDistance(newPoint, rrt->_targetPosition);
                int goalIdx = addNode(rrt, rrt->_targetPosition, newNodeIdx, goalCost);

                // Extract the path
                RRTPathExecution::extractPath(rrt, goalIdx);
                return true;
            }
        }
    }

    return false; // No path found yet
}