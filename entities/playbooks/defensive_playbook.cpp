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
    return true;
}

int DefensivePlaybook::getPriority() const {
    // Higher priority when in defensive situations
    return 100;
}

void DefensivePlaybook::initializeRoles() {
    QList<Player*> ourTeam = _worldMap->getTeam(_teamColor);
    

    
    // // Assign goalkeeper (player 0)
    assignRole(0, std::make_unique<GoalkeeperRole>(ourTeam[0], _worldMap, _ownGoalX, _ownGoalY));
    
    // Assign defender (player 1)
    assignRole(1, std::make_unique<DefenderRole>(ourTeam[1], _worldMap, _ownGoalX, _ownGoalY, _opponentGoalX, _opponentGoalY));
    
    // // Assign attacker (player 2)
    assignRole(2, std::make_unique<DefenderRole>(ourTeam[2], _worldMap, _ownGoalX, _ownGoalY, _opponentGoalX, _opponentGoalY));
}

void DefensivePlaybook::updatePlaybookState() {
    // Implement logic to defense playbook with players roles
}