#include "skill_goTo.h"
#include "player/player.h"

void GoTo::goTo(QVector2D &targetCoordinates, ActuatorClient *actuator) {

    // Initialize variables to store the last error, reversed state, and wheel speeds
    float lastError = 0.0f;      
    bool reversed = false;       
    float leftWheelSpeed;        
    float rightWheelSpeed;       

    // Get the current orientation and target angle to calculate the angle difference
    float robotAngle = _player->getOrientation();                
    float targetAngle = Basic::getAngle(_player->getCoordinates(), targetCoordinates); 

    // Check if reversing would be more efficient and update the robot's orientation if needed
    if(Basic::checkCanBeReversed(robotAngle, targetAngle)) {     
        reversed = true;                                         
        robotAngle = Basic::normalizeAngle(robotAngle + M_PI);   // Adjust orientation by 180 degrees
    }

    // Calculate the error between robot's orientation and target angle
    float angleError = Basic::smallestAngleDiff(robotAngle, targetAngle);  

    // Calculate motor speed using PD control (Proportional-Derivative)
    float motorSpeed = (kp * angleError) + (kd * (angleError - lastError));  
    lastError = angleError;  

    // Clamp motor speed to ensure it doesn't exceed maximum allowed speed
    motorSpeed = motorSpeed > BASE_SPEED ? BASE_SPEED : motorSpeed;      // Limit motor speed upper bound
    motorSpeed = motorSpeed < -BASE_SPEED ? -BASE_SPEED : motorSpeed;    // Limit motor speed lower bound

    // Adjust wheel speeds based on the direction and motor speed calculated
    if (!reversed) {  // If moving forward
        if (motorSpeed > 0) {
            leftWheelSpeed = BASE_SPEED;                     
            rightWheelSpeed = BASE_SPEED - motorSpeed;       
        } else {
            leftWheelSpeed = BASE_SPEED + motorSpeed;        
            rightWheelSpeed = BASE_SPEED;                    
        }
    } else {  // If moving in reverse
        if (motorSpeed > 0) {
            leftWheelSpeed = -BASE_SPEED + motorSpeed;       
            rightWheelSpeed = -BASE_SPEED;                   
        } else {
            leftWheelSpeed = -BASE_SPEED;                    
            rightWheelSpeed = -BASE_SPEED - motorSpeed;      
        }
    }


    actuator->sendCommand(_player->getPlayerId(), leftWheelSpeed, rightWheelSpeed);

} 

void GoTo::runSkill(ActuatorClient *actuator) {

    goTo(_targetCoordinates, actuator);

}