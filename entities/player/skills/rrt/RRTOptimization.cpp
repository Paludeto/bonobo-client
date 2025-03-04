#include "RRTOptimization.h"
#include "skill_rrt.h"  
#include "RRTCollision.h"
#include "RRTPathExecution.h"
#include "RRTOptimization.h"
#include "player/player.h"
#include "basics/basic.h"
#include <cmath>
#include <limits>
#include <algorithm>


std::vector<int> RRTOptimization::findNearNodes(RRT* rrt, const QVector2D& point, float radius) {
    std::vector<int> nearNodes;
    
    for(size_t i = 0; i < rrt->_treeNodes.size(); i++) {
        if(Basic::getDistance(point, rrt->_treeNodes[i].position) <= radius) {
            nearNodes.push_back(static_cast<int>(i));
        }
    }
    
    return nearNodes;
}

void RRTOptimization::rewireTree(RRT* rrt, int newNodeIndex) {
    // Calculate node radius based on RRT* formula of node search
    float n = static_cast<float>(rrt->_treeNodes.size());
    float dimension = 2.0f; // 2D space
    float gamma = std::pow(2.0f * (1.0f + 1.0f/dimension), 1.0f/dimension);
    float radius = std::min(gamma * std::pow(std::log(n) / n, 1.0f/dimension), RRT::DEFAULT_STEP_SIZE * 3.0f);
    
    // Find nodes near to the new node
    std::vector<int> nearNodes = findNearNodes(rrt, rrt->_treeNodes[newNodeIndex].position, radius);
    
    // For each nearby node, check if the path through the new node is shorter
    for(int nearIdx : nearNodes) {
        // Skip the parent node
        if(nearIdx == rrt->_treeNodes[newNodeIndex].parent) {
            continue;
        }
        
        // Calculate the cost of the new path
        float distance = Basic::getDistance(rrt->_treeNodes[newNodeIndex].position, rrt->_treeNodes[nearIdx].position);
        float newCost = rrt->_treeNodes[newNodeIndex].costToStart + distance;
        
        // If the new path is shorter and obstacle-free, rewire the tree
        if(newCost < rrt->_treeNodes[nearIdx].costToStart && 
           RRTCollision::isObstacleFree(rrt, rrt->_treeNodes[newNodeIndex].position, rrt->_treeNodes[nearIdx].position)) {
            
            rrt->_treeNodes[nearIdx].parent = newNodeIndex;
            rrt->_treeNodes[nearIdx].costToStart = newCost;
            
            // Propagate cost updates to children
            // Simple approach
            for(size_t i = 0; i < rrt->_treeNodes.size(); i++) {
                if(rrt->_treeNodes[i].parent == nearIdx) {
                    rrt->_treeNodes[i].costToStart = newCost + Basic::getDistance(rrt->_treeNodes[nearIdx].position, rrt->_treeNodes[i].position);
                }
            }
        }
    }
}

QList<QVector2D> RRTOptimization::simplifyPath(RRT* rrt, const QList<QVector2D>& path) {
    if (path.size() <= 2) {
        return path; // Already simple enough
    }
    
    QList<QVector2D> simplifiedPath;
    simplifiedPath.append(path.first()); // Always keep start point
    
    // Douglas-Peucker inspired algorithm for path simplification
    const float MAX_ERROR = 0.03f; // 3cm max error
    
    // Add a recursive path simplification function
    std::function<void(int, int)> simplifySegment = [&](int startIndex, int endIndex) {
        if (endIndex <= startIndex + 1) {
            return; // Nothing to simplify
        }
        
        // Find the point with maximum deviation
        float maxDeviation = 0.0f;
        int maxDeviationIndex = startIndex;
        
        QVector2D startPoint = path[startIndex];
        QVector2D endPoint = path[endIndex];
        
        // Calculate direction vector
        QVector2D directionVector = endPoint - startPoint;
        float segmentLength = directionVector.length();
        
        if (segmentLength < 0.001f) {
            // Start and end points are too close, just use middle point
            simplifiedPath.append(path[endIndex]);
            return;
        }
        
        directionVector.normalize();
        
        for (int i = startIndex + 1; i < endIndex; i++) {
            QVector2D point = path[i];
            
            // Calculate projection of point onto line
            QVector2D startToPoint = point - startPoint;
            float dotProduct = QVector2D::dotProduct(startToPoint, directionVector);
            
            // Clamp projection to line segment
            dotProduct = std::max(0.0f, std::min(dotProduct, segmentLength));
            
            // Calculate closest point on line
            QVector2D closestPointOnLine = startPoint + directionVector * dotProduct;
            
            // Calculate distance from point to line
            float deviation = Basic::getDistance(point, closestPointOnLine);
            
            // Additionally, check if skipping this point would cause collision
            bool causesCollision = !RRTCollision::isObstacleFree(rrt, 
                                      (i > startIndex + 1) ? path[i-1] : startPoint,
                                      (i < endIndex - 1) ? path[i+1] : endPoint);
            
            // Points that would cause collision have infinite deviation
            if (causesCollision) {
                deviation = std::numeric_limits<float>::max();
            }
            
            if (deviation > maxDeviation) {
                maxDeviation = deviation;
                maxDeviationIndex = i;
            }
        }
        
        // If maximum deviation is too high, keep the point and recursively simplify
        if (maxDeviation > MAX_ERROR) {
            simplifySegment(startIndex, maxDeviationIndex);
            simplifiedPath.append(path[maxDeviationIndex]);
            simplifySegment(maxDeviationIndex, endIndex);
        } else {
            // Direct path is good enough, just check if it's obstacle-free
            if (!RRTCollision::isObstacleFree(rrt, startPoint, endPoint)) {
                // If not obstacle-free, use the middle point and recursively simplify
                int midIndex = (startIndex + endIndex) / 2;
                simplifySegment(startIndex, midIndex);
                simplifiedPath.append(path[midIndex]);
                simplifySegment(midIndex, endIndex);
            }
            // Otherwise, the simplification is valid
        }
    };
    
    // Run the simplification algorithm
    simplifySegment(0, path.size() - 1);
    
    // Always add the end point
    simplifiedPath.append(path.last());
    
    return simplifiedPath;
}