#include "attacker_role.h"
#include "player/behaviors/attack_with_ball_behavior.h"
#include "player/behaviors/block_attack_behavior.h"
#include <iostream>

AttackerRole::AttackerRole(Player *player, WorldMap *worldMap, float ownGoalX, float ownGoalY, 
                         float opponentGoalX, float opponentGoalY)
    : Role(player, worldMap), 
      _ownGoalX(ownGoalX), 
      _ownGoalY(ownGoalY),
      _opponentGoalX(opponentGoalX), 
      _opponentGoalY(opponentGoalY),
      _wasControllingBall(false),
      _behaviorTransitionCounter(0)
{
    // Initialize behaviors when role is created
    initializeBehaviors();
}

void AttackerRole::initializeBehaviors() {
    
    // Add attack with ball behavior - high priority when we have the ball
    auto attackWithBallBehavior = std::make_unique<AttackWithBallBehavior>(_player, _worldMap, _ownGoalX, _ownGoalY, _opponentGoalX, _opponentGoalY);
    
    // Configure the attack behavior
    attackWithBallBehavior->setAvoidDefenseArea(true);
    
    // Add block attack behavior - used when opponent has the ball
    auto blockAttackBehavior = std::make_unique<BlockAttackBehavior>(_player, _worldMap, _ownGoalX, _ownGoalY, _opponentGoalX, _opponentGoalY);
    
    // Add behaviors to the role - order matters for priority when multiple behaviors could activate
    _behaviors.push_back(std::move(attackWithBallBehavior));  // Highest priority
    _behaviors.push_back(std::move(blockAttackBehavior));     // Secondary priority
}

void AttackerRole::updateRoleState() {
    // 1. Strategic positioning when no one has the ball
    // 2. Coordinating with other players
    // 3. Special play situations
}