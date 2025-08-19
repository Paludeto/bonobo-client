#ifndef RRT_COLLISION_H
#define RRT_COLLISION_H

#include <QVector2D>

class RRT;
class Player;

/**
 * @brief Collision detection functionality for RRT
 * 
 * Contains methods for checking obstacle clearance 
 * and predicting collisions.
 */
class RRTCollision {
public:
    /**
     * @brief Check if the path between two points is obstacle-free
     * 
     * @param rrt Reference to the main RRT object
     * @param fromPoint Starting point
     * @param toPoint End point
     * @return true if the path is clear, false otherwise
     */
    static bool isObstacleFree(RRT* rrt, const QVector2D& fromPoint, const QVector2D& toPoint);
    
    /**
     * @brief Check if predicted obstacle movements allow safe passage
     * 
     * @param rrt Reference to the main RRT object
     * @param fromPoint Starting point
     * @param toPoint End point
     * @return true if path is predicted to be clear, false otherwise
     */
    static bool areObstacleMovementsPredictablyClear(RRT* rrt, const QVector2D& fromPoint, const QVector2D& toPoint);
    
    /**
     * @brief Check if the lookahead path is clear of obstacles
     * 
     * @param rrt Reference to the main RRT object
     * @param currentPosition Current robot position
     * @param currentIndex Current index in the path
     * @param lookaheadSteps Number of steps to look ahead
     * @return true if lookahead path is clear, false otherwise
     */
    static bool isLookaheadClear(RRT* rrt, const QVector2D &currentPosition, int currentIndex, int lookaheadSteps);
};

#endif // RRT_COLLISION_H