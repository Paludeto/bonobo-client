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
    
    // Set any behavior-specific parameters
    defendGoalBehavior->setDistGoal(0.01f);  // 1cm from goal line
    
    // Add behavior to the role
    _behaviors.push_back(std::move(defendGoalBehavior));
    
    // In the future, additional behaviors could be added here
}

void GoalkeeperRole::updateRoleState() {
    // Update any role-specific state
    // In a more complete implementation, you might track:
    // - Recent ball movements
    // - Goal threat level
    // - Special game situations
}