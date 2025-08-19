#include "playbook.h"
#include <iostream>

void Playbook::executePlaybook(ActuatorClient *actuator) {
    updatePlaybookState();

    // Execute each assigned role
    for(auto &pair : _roles) {
        int playerId = pair.first;
        Role *role = pair.second.get();

        if(role) {
            // Execute the role
            role->executeRole(actuator);
            
            // Log active behavior for debugging
            Behavior* activeBehavior = role->getActiveBehavior();
        }   
    }
}

void Playbook::activate() {
    if(!_isActive) {
        _isActive = true;

        initializeRoles();
    }
}

void Playbook::deactivate() {
    if(_isActive) {
        _isActive = false;

        _roles.clear();
    }
}

void Playbook::assignRole(int playerId, std::unique_ptr<Role> role) {
    _roles[playerId] = std::move(role);
}