#include "skill_rrt.h"
#include "RRTCore.h"
#include "RRTCollision.h"
#include "RRTPathExecution.h"
#include "RRTOptimization.h"
#include "player/player.h"
#include "worldmap/worldmap.h"

#include <cmath>
#include <limits>
#include <algorithm>

static std::map<int, int> globalStuckCounters;

RRT::RRT(Player *player, QVector2D &targetPosition, WorldMap *worldMap, float robotRadius, int stuckCounter) 
    : SkillManager(player), 
      _targetPosition(targetPosition), 
      _worldMap(worldMap),
      _robotRadius(robotRadius), 
      _maxIterations(DEFAULT_MAX_ITERATIONS), 
      _currentIteration(0), 
      _pathFound(false), 
      _initialized(false), 
      _pathCompleted(false),
      _needsReplanning(false), 
      _isPerformingPredictiveManeuver(false), 
      _predictiveManeuver(nullptr), 
      _stuckCounter(stuckCounter), 
      _currentPathIndex(0), 
      _pathFollowingThreshold(0.05f), 
      _unitDist(0.0f, 1.0f)
{
    std::random_device rd;
    _rng = std::mt19937(rd());

    int playerId = player->getPlayerId();
    
    // If we already have a counter for this player, use it
    if (globalStuckCounters.find(playerId) != globalStuckCounters.end()) {
        _stuckCounter = globalStuckCounters[playerId];
    } else {
        // Otherwise, use the passed value (usually 0)
        _stuckCounter = stuckCounter;
        globalStuckCounters[playerId] = stuckCounter;
    }
    
}

void RRT::runSkill(ActuatorClient *actuator) {
    // Check if performing predictive maneuver
    if (_isPerformingPredictiveManeuver) {
        bool completed = RRTPathExecution::executePredictiveManeuver(this, actuator);
        if (completed) {
            std::cout << "RRT: Predictive maneuver completed, returning to normal operation" << std::endl;
            _isPerformingPredictiveManeuver = false;
            _predictiveManeuver.reset();
            _needsReplanning = true;
            _stuckCounter = 0;
        }
        return;
    }
    
    // Initialization or replanning
    if (!_initialized || _needsReplanning) {
        RRTCore::initializeTree(this);
        _currentIteration = 0;
        _pathFound = false;
        _pathCompleted = false;
        _needsReplanning = false;
        _initialized = true;
    }
    
    // If path found, follow it
    if (_pathFound) {
        RRTPathExecution::executePathStep(this, actuator);
    }
    else if (_currentIteration < _maxIterations) {
        bool foundPath = RRTCore::iterateRRT(this, MAX_ITERATIONS_PER_FRAME);
        if (foundPath) {
            _pathFound = true;
            _currentPathIndex = 0;
            _stuckCounter = 0;
            globalStuckCounters[_player->getPlayerId()] = 0;
        }
        else if (_currentIteration >= _maxIterations) {
            _stuckCounter++;
            globalStuckCounters[_player->getPlayerId()] = _stuckCounter;
            if (_stuckCounter >= 2) {
                std::cout << "RRT: Detected stuck condition after multiple failures, starting predictive maneuver" << std::endl;
                _isPerformingPredictiveManeuver = true;
                _predictiveManeuver = std::make_unique<PredictiveManeuver>(_player, _worldMap);
                return;
            }
            std::cout << "RRT: Direct approach to target, attempt " << _stuckCounter << "/3" << std::endl;
            _player->goTo(_targetPosition, actuator);
        }
    }
}