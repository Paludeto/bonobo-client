#include "role.h"
#include <iostream>

void Role::executeRole(ActuatorClient *actuator) {
    // Update any role-specific state before selecting a behavior
    updateRoleState();

    // Select the most appropriate behavior
    int selectedBehaviorIndex = selectBehavior();

    // Check if behavior has changed
    if(selectedBehaviorIndex != _activeBehaviorIndex) {
        // Deactivate current behavior if one is active
        if(_activeBehaviorIndex >= 0 && _activeBehaviorIndex < _behaviors.size()) {
            _behaviors[_activeBehaviorIndex]->deactivate();
        }

        // Set and activate new behavior if one was selected
        _activeBehaviorIndex = selectedBehaviorIndex;
        if(_activeBehaviorIndex >= 0 && _activeBehaviorIndex < _behaviors.size()) {
            _behaviors[_activeBehaviorIndex]->activate();
        }
    } 

    // Execute
    // Execute the active behavior if one is selected
    if (_activeBehaviorIndex >= 0 && _activeBehaviorIndex < _behaviors.size()) {
        _behaviors[_activeBehaviorIndex]->execute(actuator);
    }
}

Behavior *Role::getActiveBehavior() {
    if (_activeBehaviorIndex >= 0 && _activeBehaviorIndex < _behaviors.size()) {
        return _behaviors[_activeBehaviorIndex].get();
    }
    return nullptr;
}

int Role::selectBehavior() {
    int selectedIndex = -1;
    int highestPriority = -1;

    // First check if current behavior should remain active
    if (_activeBehaviorIndex >= 0 && _activeBehaviorIndex < _behaviors.size()) {
        Behavior* currentBehavior = _behaviors[_activeBehaviorIndex].get();
        if (currentBehavior->shouldKeepActive()) {
            return _activeBehaviorIndex;
        }
    }

    // Find the highest priority behavior that should be activated
    for (size_t i = 0; i < _behaviors.size(); i++) {
        Behavior* behavior = _behaviors[i].get();
        
        if (behavior->shouldActivate() && behavior->getPriority() > highestPriority) {
            highestPriority = behavior->getPriority();
            selectedIndex = static_cast<int>(i);
        }
    }
    
    return selectedIndex;
}