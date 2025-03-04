#include "skill_escape.h"
#include "player/player.h"

PredictiveManeuver::PredictiveManeuver(Player *player, WorldMap *worldMap)
    : SkillManager(player),
      _worldMap(worldMap),
      _maneuverComplete(false),
      _maneuverTimer(0.0f)
{
    
    std::cout << "Predictive Maneuver: Starting evasive maneuver" << std::endl;
    std::cout << "Current ball: " << _worldMap->getBallPosition().x() << ", " << _worldMap->getBallPosition().y() << std::endl;
}

void PredictiveManeuver::runSkill(ActuatorClient *actuator) {
    // Get the current position and other necessary information
    QVector2D robotPos = _player->getCoordinates();
    QVector2D ballPos = _worldMap->getBallPosition();
    float robotOrientation = _player->getOrientation();
    
    // Increment the timer
    _maneuverTimer += 0.0167f;
    
    // Vector from the ball to the robot (direction to move away)
    QVector2D escapeDirection = robotPos - ballPos;
    
    // If we are too close to the ball, the direction might be unstable
    if (escapeDirection.length() < 0.01f) {
        // Use a random direction based on time
        float randomAngle = std::fmod(_maneuverTimer * 10.0f, 2.0f * M_PI);
        escapeDirection = QVector2D(std::cos(randomAngle), std::sin(randomAngle));
    } else {
        escapeDirection.normalize();
    }
    
    // Angle of the escape direction
    float escapeAngle = std::atan2(escapeDirection.y(), escapeDirection.x());
    
    // Difference between the escape angle and the robot's orientation
    float angleDiff = Basic::smallestAngleDiff(robotOrientation, escapeAngle);
    
    // Decide the strategy based on the timer phase
    float leftSpeed, rightSpeed;
    
    if (_maneuverTimer < 2.0f) {
        // Phase 1: Move backward relative to the current orientation
        leftSpeed = -100.0f;
        rightSpeed = -100.0f;
    } 
    else if (_maneuverTimer < 3.0f) {
        // Phase 2: Rotate to align with the escape direction
        if (std::abs(angleDiff) < 0.2f) {  // If already approximately aligned
            leftSpeed = 0.0f;
            rightSpeed = 0.0f;
        } 
        else if (angleDiff > 0) {  // Turn right
            leftSpeed = 80.0f;
            rightSpeed = -80.0f;
        } 
        else {  // Turn left
            leftSpeed = -80.0f;
            rightSpeed = 80.0f;
        }
    } 
    else if (_maneuverTimer < 4.0f) {
        // Phase 3: Move forward in the escape direction
        leftSpeed = 100.0f;
        rightSpeed = 100.0f;
    } 
    else {
        // Complete the maneuver after 3 seconds
        std::cout << "PredictiveManeuver: Smart evasion completed" << std::endl;
        _maneuverComplete = true;
        return;
    }
    
    // Send command to the wheels
    actuator->sendCommand(_player->getPlayerId(), leftSpeed, rightSpeed);
}