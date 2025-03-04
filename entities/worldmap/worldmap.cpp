#include "worldmap.h"
#include <chrono>

// Implement buffering in vision.cpp to avoid networki spikes
bool WorldMap::updateFrame() {
    
    // auto start = std::chrono::high_resolution_clock::now();

    const fira_message::sim_to_ref::Environment &lastEnv = _frameUpdater->getLastEnvironment();

    if (!lastEnv.has_frame()) {
        std::cout << "No frame detected\n";
        return false;
    }

    _lastFrame = lastEnv.frame();

    updatePlayers(Color::BLUE);
    updatePlayers(Color::YELLOW);
    updateBallPosition();

    // auto end = std::chrono::high_resolution_clock::now();

    // auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    
    // std::cout << "Frame update runtime: " << duration << std::endl;

    return true;

}

void WorldMap::initializeTeamPool(QList<Player *> playerList, Color color, quint8 maxSize) {
    
    for (int i = 0; i < maxSize; ++i) {
        playerList.append(new Player(color, i)); 
    }

}


// Can be improved, there is redundancy - TODO
void WorldMap::updatePlayers(Color teamColor) {

    // Select team list and current robot count
    auto& teamList = (teamColor == Color::BLUE) ? _blueTeam : _yellowTeam;
    auto robotCount = (teamColor == Color::BLUE) ? _lastFrame.robots_blue_size() : _lastFrame.robots_yellow_size();

    // Ensure team list has enough players
    if (teamList.size() < robotCount) {

        int additionalPlayers = robotCount - teamList.size();
        for (int i = 0; i < additionalPlayers; ++i) {
            teamList.append(new Player(teamColor, teamList.size()));
        }

    }

    // Update only necessary players
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

QVector2D &WorldMap::getBallPosition() {
    
    return _ballPosition;

}

QList<Player *> WorldMap::getTeam(Color color) {

    if (color == Color::BLUE) {
        return _blueTeam;
    } else if (color == Color::YELLOW) {
        return _yellowTeam;
    } else {
        throw std::invalid_argument("Invalid team color specified");
    }
}

fira_message::sim_to_ref::Environment WorldMap::getEnvironment() {
    return _frameUpdater->getLastEnvironment();
}

