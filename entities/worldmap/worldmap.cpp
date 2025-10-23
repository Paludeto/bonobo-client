#include "worldmap.h"
#include <chrono>
#include <QDateTime>
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
        
        qint64 now = QDateTime::currentMSecsSinceEpoch();
        float newOrientation = robot.orientation();
        float oldOrientation = teamList[id]->_lastOrientation;
        qint64 lastTime = teamList[id]->_lastUpdateTime;

        if (lastTime > 0) {
            float dt = (now - lastTime) / 1000.0f; // segundos
            if (dt > 0.0f) {
                float dTheta = Basic::smallestAngleDiff(oldOrientation, newOrientation);
                teamList[id]->_angularVelocity = dTheta / dt;
            }
        }

        teamList[id]->_lastOrientation = newOrientation;
        teamList[id]->_lastUpdateTime = now;

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

    const float goalX = (color == VSSRef::BLUE) ? getMinX() : getMaxX();
    return QVector2D(goalX, getGoalWidth() / 2.0f);
}

QVector2D WorldMap::getOurLeftPost(VSSRef::Color color) const {

    const float goalX = (color == VSSRef::BLUE) ? getMinX() : getMaxX();
    return QVector2D(goalX, -getGoalWidth() / 2.0f);
}

QVector2D WorldMap::getOurGoalCenter(VSSRef::Color color) const {

    const float goalX = (color == VSSRef::BLUE) ? getMinX() : getMaxX();
    return QVector2D(goalX, 0.0f);
}

bool WorldMap::isInsideOurArea(const QVector2D& point, float factor, VSSRef::Color team) {
    const float goalX = (team == VSSRef::Color::YELLOW) ? getMaxX() : getMinX();
    const float areaLength = getAreaLength() * factor; 
    const float yHalf = std::max(getGoalWidth() * 0.5f, getAreaWidth()) * factor; 

    float xMin, xMax;
    if (team == VSSRef::Color::YELLOW) {
        xMin = goalX - areaLength;
        xMax = goalX;
    } else {
        xMin = goalX;
        xMax = goalX + areaLength;
    }

    const bool insideX = (point.x() >= xMin && point.x() <= xMax);
    const bool insideY = (point.y() >= -yHalf && point.y() <= yHalf);
    return insideX && insideY;
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
    
    float controlThreshold = 0.09;
    
    return distance <= controlThreshold;
}

bool WorldMap::isOurTeamWithBall(Color teamColor) const {
    QList<Player*> team = (teamColor == Color::BLUE) ? _blueTeam : _yellowTeam;
    for (Player* player : team) {
        if(player->getPlayerId() == 0) {
            continue;
        }

        if(isPlayerControllingBall(player)){
            return true;  
        } 
    }

    return false;
}

Player* WorldMap::getPlayerClosestToBall(Color teamColor) const {
    // Get the team
    QList<Player*> team = (teamColor == Color::BLUE) ? _blueTeam : _yellowTeam;
    
    // Find closest player
    Player* closestPlayer = nullptr;
    float minDistance = std::numeric_limits<float>::max();
    
    const bool hasAlternatives = team.size() > 1;

    for (Player* player : team) {
        if (!player) {
            continue;
        }

        if (player->getPlayerId() == 0 && hasAlternatives) {
            continue;
        }

        float distance = Basic::getDistance(player->getCoordinates(), _ballPosition);

        if (distance < minDistance) {
            minDistance = distance;
            closestPlayer = player;
        }
    }

    if (!closestPlayer) {
        for (Player* player : team) {
            if (player) {
                closestPlayer = player;
                break;
            }
        }
    }

    return closestPlayer;
}