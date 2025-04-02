#include "offensive_playbook.h"
#include "player/roles/goalkeeper_role.h"
#include "player/roles/defender_role.h"
#include "player/roles/attacker_role.h"
#include "basics/basic.h"
#include <iostream>

OffensivePlaybook::OffensivePlaybook(WorldMap *worldMap, VSSRef::Color teamColor, float ownGoalX, float ownGoalY, float opponentGoalX, float opponentGoalY)
                                    : Playbook(worldMap, teamColor), _ownGoalX(ownGoalX), _ownGoalY(ownGoalY), _opponentGoalX(opponentGoalX),
                                    _opponentGoalY(opponentGoalY) {}

bool OffensivePlaybook::shouldActivate() {
    // Activate when ball is in opponent's half or our team has the ball
    return _worldMap->isBallInTheirSide(_teamColor) || isTeamWithBall();
}

int OffensivePlaybook::getPriority() const {
    // Higher priority when in offensive situations
    return 90; // Slightly lower than defensive to prioritize defense when uncertain
}

void OffensivePlaybook::initializeRoles() {
    
    // Get our team
    QList<Player*> ourTeam = _worldMap->getTeam(_teamColor);
    
    // Make sure we have enough players
    if (ourTeam.size() < 3) {
        return;
    }
    
    // Assign goalkeeper (player 0)
    assignRole(0, std::make_unique<GoalkeeperRole>(ourTeam[0], _worldMap, _ownGoalX, _ownGoalY));
    
    // In offensive playbook, we have one defender and one attacker
    
    // Assign defender (player 1) - will still move up on offense
    assignRole(1, std::make_unique<DefenderRole>(ourTeam[1], _worldMap, _ownGoalX, _ownGoalY, _opponentGoalX, _opponentGoalY));
    
    // Assign attacker (player 2)
    assignRole(2, std::make_unique<AttackerRole>(ourTeam[2], _worldMap, _ownGoalX, _ownGoalY, _opponentGoalX, _opponentGoalY));
}

void OffensivePlaybook::updatePlaybookState() {
    // Implement logic to attack playbook with players roles
}

bool OffensivePlaybook::isTeamWithBall() const { // We can implement this function in WorldMap to all playbooks use
    // Get the ball position
    QVector2D ballPosition = _worldMap->getBallPosition();
    
    // Get our team
    QList<Player*> ourTeam = _worldMap->getTeam(_teamColor);
    
    // Check if any of our players is close to the ball
    for (Player* player : ourTeam) {
        float distance = Basic::getDistance(player->getCoordinates(), ballPosition);
        if (distance < BALL_CONTROL_THRESHOLD) {
            return true;
        }
    }
    
    return false;
}