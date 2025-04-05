#ifndef WORLDMAP_H
#define WORLDMAP_H

#include "vision/vision.h"
#include "player/player.h"
#include "vssref_common.pb.h"
#include <QVector2D>

#define MAX_PLAYERS 3

class WorldMap

{

public:

    // Lazy initializing lists, object pooling technique
    WorldMap(VisionClient *visionClient) : _frameUpdater(visionClient) 
    {
        initializeTeamPool(_blueTeam, Color::BLUE, MAX_PLAYERS);
        initializeTeamPool(_yellowTeam, Color::YELLOW, MAX_PLAYERS);
    }

    ~WorldMap() {
        qDeleteAll(_blueTeam);   
        qDeleteAll(_yellowTeam);  
    }

    // All of these should be boolean values and need to be checked -TODO
    void updateBallPosition();
    void updatePlayers(Color color);
    bool updateFrame();

    QList<Player *> getTeam(Color color);

    QVector2D &getBallPosition();
    QVector2D getBallVelocity();

    fira_message::sim_to_ref::Environment getEnvironment();

    // Field dimensions (in meters)
    float getFieldLength() const { return 1.5f; } // 1.5m
    float getFieldWidth() const { return 1.3f; }  // 1.3m
    
    // Field limits
    float getMinX() const { return -getFieldLength()/2.0f; }
    float getMaxX() const { return getFieldLength()/2.0f; }
    float getMinY() const { return -getFieldWidth()/2.0f; }
    float getMaxY() const { return getFieldWidth()/2.0f; }
    
    // Goal dimensions
    float getGoalWidth() const { return 0.4f; }  // 40cm
    float getAreaLength() const { return 0.7f; }
    float getAreaWidth() const { return 0.15f; }
    
    // Goal positions for BLUE team (goal at positive X)
    QVector2D getBlueGoalCenter() const { return QVector2D(getMaxX(), 0.0f); }

    QVector2D getOurRightPost(VSSRef::Color color) const;
    QVector2D getOurLeftPost(VSSRef::Color color) const;
    QVector2D getOurGoalCenter(VSSRef::Color color) const;
    
    // Goal positions for YELLOW team (goal at negative X)
    QVector2D getYellowGoalCenter() const { return QVector2D(getMinX(), 0.0f); }
    
    // Ball and player properties
    float getRobotRadius() const { return 0.056f; }  
    
    // Helper methods for behaviors
    bool isBallInOurSide(Color ourTeam) const {
        QVector2D ballPos = _ballPosition;
        return (ourTeam == Color::BLUE) ? (ballPos.x() > 0) : (ballPos.x() <= 0);
    }

    bool isOurSideLeft(Color ourTeam) const {
        return (ourTeam == Color::BLUE) ? (false) : (true);
    }
    
    bool isBallInTheirSide(Color ourTeam) const {
        return !isBallInOurSide(ourTeam);
    }
    
    // Team perception methods
    bool isTeammateNearerToBall(Player* player) const;
    bool isPlayerControllingBall(Player* player) const;
    Player* getPlayerClosestToBall(Color teamColor) const;

private:

    // Object pooling (reduces allocation and deletion)
    void initializeTeamPool(QList<Player *> playerList, Color color, quint8 maxSize);
    
    fira_message::Frame _lastFrame;
    QList<Player *> _blueTeam;
    QList<Player *> _yellowTeam;
    VisionClient *_frameUpdater;
    QVector2D _ballPosition;
    QVector2D _prevBallPosition;

};

#endif // WORLDMAP_H