#include "strategy.h"
#include "playbooks/defensive_playbook.h"
#include "playbooks/offensive_playbook.h"

// Game state constants
enum GameState {
    NORMAL_PLAY = 0,
    KICKOFF = 1,
    FREEKICK = 2,
    PENALTY = 3
};

Strategy::Strategy(WorldMap *worldMap, ActuatorClient *actuator, VSSRef::Color teamColor, float ownGoalX, float ownGoalY, float opponentGoalX, float opponentGoalY) : _worldMap(worldMap),
                    _actuator(actuator), _teamColor(teamColor), _ownGoalX(ownGoalX), _ownGoalY(ownGoalY), _opponentGoalX(opponentGoalX), _opponentGoalY(opponentGoalY),
                    _activePlaybook(nullptr), _gameState(NORMAL_PLAY)
{
    initializePlaybooks();
}

void Strategy::executeStrategy() {
    updateGameState();
    
    Playbook* selectedPlaybook = selectPlaybook();
    
    // If playbook has changed, handle the transition
    if (selectedPlaybook != _activePlaybook) {
        // Deactivate current playbook if one is active
        if (_activePlaybook) {
            _activePlaybook->deactivate();
        }
        
        // Set and activate the new playbook
        _activePlaybook = selectedPlaybook;
        
        if (_activePlaybook) {
            _activePlaybook->activate();
        }
    }
    
    // Execute the active playbook if one is selected
    if (_activePlaybook) {
        _activePlaybook->executePlaybook(_actuator);
    } else {
    }
}

void Strategy::initializePlaybooks() {
    // Create all available playbooks
    
    // Defensive playbook
    _playbooks.push_back(std::make_unique<DefensivePlaybook>(_worldMap, _teamColor, _ownGoalX, _ownGoalY, _opponentGoalX, _opponentGoalY));
    
    // Offensive playbook
    _playbooks.push_back(std::make_unique<OffensivePlaybook>(_worldMap, _teamColor, _ownGoalX, _ownGoalY, _opponentGoalX, _opponentGoalY));
    
    // - KickoffPlaybook: For kickoff situations
    // - FreeKickPlaybook: For free kick situations
    // - PenaltyPlaybook: For penalty situations
    // - CounterAttackPlaybook: For quick transitions
}

Playbook* Strategy::selectPlaybook() {
    if(_worldMap->isBallInTheirSide(_teamColor)) {
        return _playbooks[1].get();
    } else {
        return _playbooks[0].get();
    }
}

void Strategy::updateGameState() {
    // In a real implementation, this would determine the game state
    // based on referee commands and field conditions
    
    // For simplicity, we'll just use NORMAL_PLAY for now
    _gameState = NORMAL_PLAY;
    
    // Detect kickoff situation (ball at center, not moving)
    QVector2D ballPos = _worldMap->getBallPosition();
    bool ballAtCenter = ballPos.length() < 0.05f; // 5cm from center
    
    // For a complete implementation, we would check ball velocity too
    // and also check referee signals from the referee client
    
    if (ballAtCenter) {
        _gameState = KICKOFF;
    }
    
    // Additional game state detection would go here for:
    // - Free kicks
    // - Penalties
    // - Goal kicks
    // - Other special situations
}