#include "RRTCollision.h"
#include "skill_rrt.h"
#include "player/player.h"
#include "worldmap/worldmap.h"

bool RRTCollision::isObstacleFree(RRT* rrt, const QVector2D& fromPoint, const QVector2D& toPoint) {
    // Get a list of all players on the field
    QList<Player*> blueTeam = rrt->_worldMap->getTeam(VSSRef::Color::BLUE);
    QList<Player*> yellowTeam = rrt->_worldMap->getTeam(VSSRef::Color::YELLOW);

    // Function to check collision
    auto checkCollision = [&](const QVector2D& point, const QVector2D& playerPos) {
        return Basic::getDistance(point, playerPos) <= (rrt->_robotRadius * 2.0f);
    };

    // Check a discrete number of points along the path
    float pathLength = Basic::getDistance(fromPoint, toPoint);
    int checkSteps = static_cast<int>(pathLength / (RRT::DEFAULT_STEP_SIZE * 0.5f)) + 1;

    for(int i = 0; i <= checkSteps; i++) {
        float t = static_cast<float>(i) / static_cast<float>(checkSteps);
        QVector2D checkPoint = fromPoint * (1.0f - t) + toPoint * t;

        // Check collision with blueTeam players
        for(const auto& player : blueTeam) {
            if(player != rrt->_player) { // ignore itself
                if(checkCollision(checkPoint, player->getCoordinates())) {
                    return false; // There's collision
                }
            }
        }

        // Check collision with yellowTeam players
        for(const auto& player : yellowTeam) {
            if(player != rrt->_player) { // ignore itself
                if(checkCollision(checkPoint, player->getCoordinates())) {
                    return false; // There's collision
                }
            }
        }
    }       

    return true; // No collision 
}

bool RRTCollision::areObstacleMovementsPredictablyClear(RRT* rrt, const QVector2D &fromPoint, const QVector2D &toPoint) {
    // Get players and their velocities
    QList<Player*> blueTeam = rrt->_worldMap->getTeam(VSSRef::Color::BLUE);
    QList<Player*> yellowTeam = rrt->_worldMap->getTeam(VSSRef::Color::YELLOW);
    
    // Estimated time for our robot to reach the point
    float distanceToTarget = Basic::getDistance(fromPoint, toPoint);
    float estimatedTimeToTarget = distanceToTarget / (rrt->_player->getLinearSpeed() + 0.001f); // Avoid division by zero
    
    // Limit prediction to a reasonable time
    const float MAX_PREDICTION_TIME = 1.0f; // seconds
    if (estimatedTimeToTarget > MAX_PREDICTION_TIME) {
        estimatedTimeToTarget = MAX_PREDICTION_TIME;
    }
    
    // Check for possible collisions with other players based on their velocities
    auto checkPredictedCollision = [&](Player* otherPlayer) {
        if (otherPlayer == rrt->_player) return false; // Ignore own robot
        
        // Current position of the other player
        QVector2D otherPos = otherPlayer->getCoordinates();
        
        // Estimated velocity of the other player
        QVector2D velocity = otherPlayer->getVelocity();
        
        // Predicted position of the other player
        QVector2D predictedPos = otherPos + velocity * estimatedTimeToTarget;
        
        // Check if there will be a collision on the path
        QVector2D direction = toPoint - fromPoint;
        float pathLength = direction.length();
        if (pathLength < 0.001f) return false;
        
        direction.normalize();
        
        // Check several points along the path
        const int numChecks = 5;
        for (int i = 0; i <= numChecks; i++) {
            float t = static_cast<float>(i) / numChecks;
            QVector2D checkPoint = fromPoint + direction * (pathLength * t);
            
            // Interpolate the obstacle position at this time
            QVector2D obstaclePos = otherPos + velocity * (estimatedTimeToTarget * t);
            
            if (Basic::getDistance(checkPoint, obstaclePos) <= (rrt->_robotRadius * 2.5f)) {
                return true; // Predicted collision
            }
        }
        
        return false;
    };
    
    // Check all players
    for (auto player : blueTeam) {
        if (checkPredictedCollision(player)) return false;
    }
    
    for (auto player : yellowTeam) {
        if (checkPredictedCollision(player)) return false;
    }
    
    return true;
}

bool RRTCollision::isLookaheadClear(RRT* rrt, const QVector2D &currentPosition, int currentIndex, int lookaheadSteps) {
    // Check sequential segments as before
    int finalIndex = std::min(currentIndex + lookaheadSteps, static_cast<int>(rrt->_finalPath.size()) - 1);
    QVector2D startPoint = currentPosition;
    
    // Verify sequential segments
    for (int i = currentIndex; i <= finalIndex; i++) {
        QVector2D targetPoint = rrt->_finalPath[i];
        if (!isObstacleFree(rrt, startPoint, targetPoint)) {
            return false;  // Obstacle detected in segment
        }
        startPoint = targetPoint;
    }
    
    // Also check direct paths to future points
    // to better detect possible shortcuts or blockages
    for (int i = currentIndex + 1; i <= finalIndex; i++) {
        if (!isObstacleFree(rrt, currentPosition, rrt->_finalPath[i])) {
            // If direct path is blocked, it's not necessarily a problem
            // Just log for debugging
            std::cout << "RRT: Direct path to waypoint " << i << " is blocked\n";
        }
    }
    
    // Check predicted movements of obstacles
    if (!areObstacleMovementsPredictablyClear(rrt, currentPosition, rrt->_finalPath[currentIndex])) {
        std::cout << "RRT: Possible future collision detected\n";
        return false;
    }
    
    return true;
}