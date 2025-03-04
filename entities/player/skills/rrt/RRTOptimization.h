#ifndef RRT_OPTIMIZATION_H
#define RRT_OPTIMIZATION_H

#include <QVector2D>
#include <vector>
#include <QList>

class RRT;

/**
 * @brief Optimization functionality for RRT* algorithm
 * 
 * Contains methods for path optimization using RRT*.
 */
class RRTOptimization {
public:
    /**
     * @brief Find nodes near a given point within a specified radius
     * 
     * @param rrt Reference to the main RRT object
     * @param point Center point
     * @param radius Search radius
     * @return Vector of indices of nearby nodes
     */
    static std::vector<int> findNearNodes(RRT* rrt, const QVector2D& point, float radius);
    
    /**
     * @brief Rewire the tree to optimize paths (RRT* specific)
     * 
     * @param rrt Reference to the main RRT object
     * @param newNodeIndex Index of the newly added node
     */
    static void rewireTree(RRT* rrt, int newNodeIndex);
    
    /**
     * @brief Simplify a path by removing unnecessary waypoints
     * 
     * @param rrt Reference to the main RRT object
     * @param path Path to simplify
     * @return Simplified path
     */
    static QList<QVector2D> simplifyPath(RRT* rrt, const QList<QVector2D>& path);
};

#endif // RRT_OPTIMIZATION_H