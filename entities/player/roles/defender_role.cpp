#include "defender_role.h"
#include "player/behaviors/block_attack_behavior.h"
#include "player/behaviors/attack_with_ball_behavior.h"
#include <iostream>

DefenderRole::DefenderRole(Player *player, WorldMap *worldMap, float ownGoalX, float ownGoalY, 
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

void DefenderRole::initializeBehaviors() {    
    // Add attack with ball behavior - activates when defender has the ball
    auto attackWithBallBehavior = std::make_unique<AttackWithBallBehavior>(_player, _worldMap, _ownGoalX, _ownGoalY, _opponentGoalX, _opponentGoalY);
    
    // Add block attack behavior - primary defensive behavior
    auto blockAttackBehavior = std::make_unique<BlockAttackBehavior>(_player, _worldMap, _ownGoalX, _ownGoalY, _opponentGoalX, _opponentGoalY);
    
    // Add behaviors to the role - order matters for priority when multiple behaviors could activate
    _behaviors.push_back(std::move(blockAttackBehavior));     // Default defensive behavior
    
    // Note: The above priority means the defender will switch to attack mode when they
    // gain possession of the ball, but will return to defensive duties otherwise
}

void DefenderRole::updateRoleState() {
    // 1. Strategic positioning when no one has the ball
    // 2. Coordinating with other players
    // 3. Special play situations
}