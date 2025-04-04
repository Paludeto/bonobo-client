#include "worldmap.h"
#include <chrono>
#include "basics/basic.h"

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
    _prevBallPosition = _ballPosition;
    _ballPosition = QVector2D(_lastFrame.ball().x(), _lastFrame.ball().y());
}

QVector2D &WorldMap::getBallPosition() {
    
    return _ballPosition;

}

QVector2D WorldMap::getBallVelocity() {
    return (_ballPosition - _prevBallPosition) / (1/60);
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

QVector2D WorldMap::getOurRightPost(VSSRef::Color color) const {

    if(color == VSSRef::BLUE) {
        return QVector2D(getMaxX(), getGoalWidth()/2.0f);
    } else {
        return QVector2D(getMinX(), -getGoalWidth()/2.0f);
    }
}

QVector2D WorldMap::getOurLeftPost(VSSRef::Color color) const {

    if(color == VSSRef::BLUE) {
        return QVector2D(getMaxX(), -getGoalWidth()/2.0f);
    } else {
        return QVector2D(getMinX(), -getGoalWidth()/2.0f);
    }
}

QVector2D WorldMap::getOurGoalCenter(VSSRef::Color color) const {

    if(color == VSSRef::BLUE) {
        return QVector2D(getMaxX(), 0.0f);
    } else {
        return QVector2D(getMinX(), 0.0f);
    }
}

// Team perception methods implementation
bool WorldMap::isTeammateNearerToBall(Player* player) const {
    // Get the ball position
    QVector2D ballPos = _ballPosition;
    
    // Get player position and distance to ball
    QVector2D playerPos = player->getCoordinates();
    float playerDistToBall = Basic::getDistance(playerPos, ballPos);
    
    // Get player's team
    Color teamColor = player->getPlayerColor();
    QList<Player*> team = (teamColor == Color::BLUE) ? _blueTeam : _yellowTeam;
    
    // Check if any teammate is closer to the ball
    for (Player* teammate : team) {
        // Skip the player itself
        if (teammate->getPlayerId() == player->getPlayerId()) {
            continue;
        }
        
        QVector2D teammatePos = teammate->getCoordinates();
        float teammateDistToBall = Basic::getDistance(teammatePos, ballPos);
        
        // If teammate is closer to the ball
        if (teammateDistToBall < playerDistToBall) {
            return true;
        }
    }
    
    return false;
}

bool WorldMap::isPlayerControllingBall(Player* player) const {
    // Get ball and player positions
    QVector2D ballPos = _ballPosition;
    QVector2D playerPos = player->getCoordinates();
    
    // Calculate distance
    float distance = Basic::getDistance(playerPos, ballPos);
    
    // Ball control threshold (robot radius + ball radius + margin)
    float controlThreshold = getRobotRadius() + getBallRadius() + 0.01f; // 1cm extra margin
    
    return distance <= controlThreshold;
}

Player* WorldMap::getPlayerClosestToBall(Color teamColor) const {
    // Get the team
    QList<Player*> team = (teamColor == Color::BLUE) ? _blueTeam : _yellowTeam;
    
    // Find closest player
    Player* closestPlayer = nullptr;
    float minDistance = std::numeric_limits<float>::max();
    
    for (Player* player : team) {
        float distance = Basic::getDistance(player->getCoordinates(), _ballPosition);
        
        if (distance < minDistance) {
            minDistance = distance;
            closestPlayer = player;
        }
    }
    
    return closestPlayer;
}