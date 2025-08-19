#include "coach.h"
#include <iostream>

Coach::Coach(WorldMap *worldMap, ActuatorClient *actuator, Color color) : _wm(worldMap), _actuator(actuator), _ourColor(color) {
    _actuator->setTeamColor(color);
    
    // Determine goal positions based on team color
    float fieldHalfWidth = 0.75f; // Half of the field width (1.5m)
    
    // Set our goal and opponent goal positions
    if (color == Color::BLUE) {
        // Blue team has goal on the right side (+x)
        _ownGoalX = -fieldHalfWidth;
        _ownGoalY = 0.0f;
        _opponentGoalX = fieldHalfWidth;
        _opponentGoalY = 0.0f;
    } else {
        // Yellow team has goal on the left side (-x)
        _ownGoalX = fieldHalfWidth;
        _ownGoalY = 0.0f;
        _opponentGoalX = -fieldHalfWidth;
        _opponentGoalY = 0.0f;
    }
    
    // Initialize the strategy
    _strategy = std::make_unique<Strategy>(_wm, _actuator, _ourColor, _ownGoalX, _ownGoalY, _opponentGoalX, _opponentGoalY);
}

void Coach::setTeam(Color color) {
    _ourTeam = _wm->getTeam(color);
}

void Coach::runCoach() {
    // Update our team reference
    setTeam(_ourColor);
    
    // Execute the strategy, which will cascade down the control hierarchy
    _strategy->executeStrategy();
    
    // - Handle referee commands
    // - Manage game state transitions
    // - Adapt strategies based on opponent behavior
}