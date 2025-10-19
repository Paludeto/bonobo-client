#include "coach.h"
#include <iostream>

Coach::Coach(WorldMap *worldMap, ActuatorClient *actuator, Color color, VSSRef::Foul foulState) : _wm(worldMap), _actuator(actuator), _ourColor(color), _foulState(foulState){
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

    if(_foulState == VSSRef::Foul::GAME_ON) {
        // Execute the strategy, which will cascade down the control hierarchy
        _strategy->executeStrategy();
    }

    if (_foulState == VSSRef::Foul::STOP ||
        _foulState == VSSRef::Foul::PENALTY_KICK ||
        _foulState == VSSRef::Foul::KICKOFF ||
        _foulState == VSSRef::Foul::HALT ||
        _foulState == VSSRef::Foul::GOAL_KICK ||
        _foulState == VSSRef::Foul::FREE_KICK ||
        _foulState == VSSRef::Foul::FREE_BALL) 
    {
        _actuator->sendCommand(0, 0, 0);
        _actuator->sendCommand(1, 0, 0);
        _actuator->sendCommand(2, 0, 0);
    }

    

    
    // - Handle referee commands
    // - Manage game state transitions
    // - Adapt strategies based on opponent behavior
}

void Coach::updateFoulState(VSSRef::Foul newState) {
    _foulState = newState;
}