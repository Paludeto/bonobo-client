#ifndef WORLDMAP_H
#define WORLDMAP_H

#include "vision/vision.h"
#include "player/player.h"
#include "vssref_common.pb.h"
#include <QVector2D>

class WorldMap

{

public:

    WorldMap(VisionClient *visionClient) : _frameUpdater(visionClient) 
    {};

    void updateBallPosition();
    void updatePlayers(Color color);
    void updateFrame();

public:

    fira_message::Frame _lastFrame;
    QList<Player> _blueTeam;
    QList<Player> _yellowTeam;
    VisionClient *_frameUpdater;
    QVector2D _ballPosition;

};

#endif // WORLDMAP_H