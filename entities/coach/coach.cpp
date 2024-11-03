#include "coach.h"


void Coach::setTeam(Color color) {

    _ourTeam = _wm->getTeam(color);

}

void Coach::runCoach() {

    setTeam(_ourColor);

    _ourTeam[0]->goTo(_wm->getBallPosition(), _actuator);
    _ourTeam[1]->rotateTo(_wm->getBallPosition(), _actuator);

}