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

    fira_message::sim_to_ref::Environment getEnvironment();

private:

    // Object pooling (reduces allocation and deletion)
    void initializeTeamPool(QList<Player *> playerList, Color color, quint8 maxSize);
    
    fira_message::Frame _lastFrame;
    QList<Player *> _blueTeam;
    QList<Player *> _yellowTeam;
    VisionClient *_frameUpdater;
    QVector2D _ballPosition;

};

#endif // WORLDMAP_H