#include "coach.h"

void Coach::runCoach(Color color) {

    _actuator->setTeamColor(color);

    if (color == Color::BLUE) {
        _ourTeam = _wm->_blueTeam;
    } else {
        _ourTeam = _wm->_yellowTeam;
    }

    _ourTeam[0]->goTo(_wm->_ballPosition, _actuator);

}