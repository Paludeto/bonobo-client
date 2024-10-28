#include "worldmap.h"

void WorldMap::updateFrame() {
    
    fira_message::sim_to_ref::Environment lastEnv = _frameUpdater->getLastEnvironment();

    if (!lastEnv.has_frame()) {
        std::cout << "No frame detected\n";
        return;
    }

    _lastFrame = lastEnv.frame();

    updatePlayers(Color::BLUE);
    updatePlayers(Color::YELLOW);
    updateBallPosition();
    
}

void WorldMap::updatePlayers(Color teamColor) {

    if (teamColor == Color::BLUE) {
        
        if (_blueTeam.size() != _lastFrame.robots_blue_size()) {

            _blueTeam.clear();

            for (int id = 0; id < _lastFrame.robots_blue_size(); id++) {
                _blueTeam.append(Player(Color::BLUE, id));
            }

        }

        for (int id = 0; id < _lastFrame.robots_blue_size(); id++) {

            _blueTeam[id]._coordinates = QVector2D(_lastFrame.robots_blue(id).x(), _lastFrame.robots_blue(id).y());
            _blueTeam[id]._orientation = _lastFrame.robots_blue(id).orientation();
            _blueTeam[id]._vX = _lastFrame.robots_blue(id).vx();
            _blueTeam[id]._vY = _lastFrame.robots_blue(id).vy();

        }

    } else {
      
        if (_yellowTeam.size() != _lastFrame.robots_yellow_size()) {

            _yellowTeam.clear();

            for (int id = 0; id < _lastFrame.robots_yellow_size(); id++) {
                _yellowTeam.append(Player(Color::YELLOW, id));

            }

        }

        for (int id = 0; id < _lastFrame.robots_yellow_size(); id++) {

            _yellowTeam[id]._coordinates = QVector2D(_lastFrame.robots_yellow(id).x(), _lastFrame.robots_yellow(id).y());
            _yellowTeam[id]._orientation = _lastFrame.robots_yellow(id).orientation();
            _yellowTeam[id]._vX = _lastFrame.robots_yellow(id).vx();
            _yellowTeam[id]._vY = _lastFrame.robots_yellow(id).vy();

        }

    }
    
}

void WorldMap::updateBallPosition() {

    _ballPosition = QVector2D(_lastFrame.ball().x(), _lastFrame.ball().y());

}
