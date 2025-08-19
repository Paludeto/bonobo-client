#ifndef RRT_CORE_H
#define RRT_CORE_H

#include <QVector2D>
#include <vector>
#include <QList>

class RRT;

/**
 * @brief Core functionality for the RRT algorithm
 * 
 * Contains methods for tree initialization, node sampling, 
 * and tree growth.
 */
class RRTCore {
public:
    /**
     * @brief Initialize the RRT tree with the start position
     * 
     * @param rrt Reference to the main RRT object
     */
    static void initializeTree(RRT* rrt);
    
    /**
     * @brief Sample a random point in the configuration space
     * 
     * @param rrt Reference to the main RRT object
     * @return Random point as QVector2D
     */
    static QVector2D sampleRandomPoint(RRT* rrt);
    
    /**
     * @brief Find the nearest node in the tree to a given point
     * 
     * @param rrt Reference to the main RRT object
     * @param point Point to find the nearest node to
     * @return Index of the nearest node
     */
    static int findNearestNode(RRT* rrt, const QVector2D& point);
    
    /**
     * @brief Steer from one point toward another with limited distance
     * 
     * @param rrt Reference to the main RRT object
     * @param fromPoint Starting point
     * @param toPoint Target point
     * @return New point that is at most stepSize away from fromPoint
     */
    static QVector2D steer(RRT* rrt, const QVector2D& fromPoint, const QVector2D& toPoint);
    
    /**
     * @brief Add a new node to the tree
     * 
     * @param rrt Reference to the main RRT object
     * @param position Position of the new node
     * @param parent Index of the parent node
     * @param cost Cost to reach this node from the start
     * @return Index of the newly added node
     */
    static int addNode(RRT* rrt, const QVector2D& position, int parent, float cost);
    
    /**
     * @brief Perform a fixed number of RRT iterations
     * 
     * @param rrt Reference to the main RRT object
     * @param iterations Number of iterations to perform
     * @return true if goal was reached, false otherwise
     */
    static bool iterateRRT(RRT* rrt, int iterations);
};

#endif // RRT_CORE_H