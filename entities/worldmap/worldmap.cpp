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

    auto& teamList = (teamColor == Color::BLUE) ? _blueTeam : _yellowTeam;
    auto robotCount = (teamColor == Color::BLUE) ? _lastFrame.robots_blue_size() : _lastFrame.robots_yellow_size();

    // Resize team if needed
    if (teamList.size() != robotCount) {

        qDeleteAll(teamList);
        teamList.clear();
        for (int id = 0; id < robotCount; ++id) {
            teamList.append(new Player(teamColor, id));
        }
        
    }

    // Update player data without copying
    for (int id = 0; id < robotCount; ++id) {
        auto& robot = (teamColor == Color::BLUE) ? _lastFrame.robots_blue(id) : _lastFrame.robots_yellow(id);
        teamList[id]->_coordinates = QVector2D(robot.x(), robot.y());
        teamList[id]->_orientation = robot.orientation();
        teamList[id]->_vX = robot.vx();
        teamList[id]->_vY = robot.vy();
    }

}

void WorldMap::updateBallPosition() {

    _ballPosition = QVector2D(_lastFrame.ball().x(), _lastFrame.ball().y());

}
