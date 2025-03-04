#include "coach.h"
#include <iostream>

void Coach::setTeam(Color color) {
    _ourTeam = _wm->getTeam(color);
}

void Coach::runCoach() {
    setTeam(_ourColor);

    _ourTeam[0]->pathPlanning(_wm->getBallPosition(), _wm, 0.05f, _actuator);
}