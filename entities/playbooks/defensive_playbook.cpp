#include "defensive_playbook.h"
#include "player/roles/goalkeeper_role.h"
#include "player/roles/defender_role.h"
#include "player/roles/attacker_role.h"
#include "basics/basic.h"
#include <iostream>

DefensivePlaybook::DefensivePlaybook(WorldMap *worldMap, VSSRef::Color teamColor, float ownGoalX, float ownGoalY, float opponentGoalX, 
                                    float opponentGoalY) : Playbook(worldMap, teamColor), _ownGoalX(ownGoalX), _ownGoalY(ownGoalY), _opponentGoalX(opponentGoalX),
                                    _opponentGoalY(opponentGoalY) {}

bool DefensivePlaybook::shouldActivate() {
    // Activate when ball is in our half or opponent has the ball
    return _worldMap->isBallInOurSide(_teamColor) || isOpponentWithBall();
}

int DefensivePlaybook::getPriority() const {
    // Higher priority when in defensive situations
    return 100;
}

void DefensivePlaybook::initializeRoles() {
    QList<Player*> ourTeam = _worldMap->getTeam(_teamColor);
    
    // Make sure we have enough players
    if (ourTeam.size() < 3) {
        return;
    }
    
    // Assign goalkeeper (player 0)
    assignRole(0, std::make_unique<GoalkeeperRole>(ourTeam[0], _worldMap, _ownGoalX, _ownGoalY));
    
    // Assign defender (player 1)
    assignRole(1, std::make_unique<AttackerRole>(ourTeam[1], _worldMap, _ownGoalX, _ownGoalY, _opponentGoalX, _opponentGoalY));
    
    // Assign attacker (player 2)
    assignRole(2, std::make_unique<AttackerRole>(ourTeam[2], _worldMap, _ownGoalX, _ownGoalY, _opponentGoalX, _opponentGoalY));
}

void DefensivePlaybook::updatePlaybookState() {
    // Implement logic to defense playbook with players roles
}

bool DefensivePlaybook::isOpponentWithBall() const {
    // Get the ball position
    QVector2D ballPosition = _worldMap->getBallPosition();
    
    // Get the opponent team
    VSSRef::Color opponentColor = (_teamColor == VSSRef::Color::BLUE) ? VSSRef::Color::YELLOW : VSSRef::Color::BLUE;
    QList<Player*> opponentTeam = _worldMap->getTeam(opponentColor);
    
    // Check if any opponent is close to the ball
    for (Player* opponent : opponentTeam) {
        float distance = Basic::getDistance(opponent->getCoordinates(), ballPosition);
        if (distance < BALL_CONTROL_THRESHOLD) {
            return true;
        }
    }
    
    return false;
}