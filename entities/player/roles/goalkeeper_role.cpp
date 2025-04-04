#include "goalkeeper_role.h"
#include "player/behaviors/defend_goal_behavior.h"
#include <iostream>

GoalkeeperRole::GoalkeeperRole(Player *player, WorldMap *worldMap, float ownGoalX, float ownGoalY)
    : Role(player, worldMap), _ownGoalX(ownGoalX), _ownGoalY(ownGoalY)
{
    // Initialize behaviors when role is created
    initializeBehaviors();
}

void GoalkeeperRole::initializeBehaviors() {
    std::cout << "Goalkeeper Role: Initializing behaviors" << std::endl;
    
    // Create and add the defend goal behavior
    auto defendGoalBehavior = std::make_unique<DefendGoalBehavior>(_player, _worldMap, _ownGoalX, _ownGoalY);
    
    // Add behavior to the role
    _behaviors.push_back(std::move(defendGoalBehavior));
    
    // In the future, additional behaviors could be added here
}

void GoalkeeperRole::updateRoleState() {
    // implement different dynamics, to leave the goal when the ball is coming towards the goalkeeper or goalkeeper attack, something like that
}