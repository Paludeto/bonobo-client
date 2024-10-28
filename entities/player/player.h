#ifndef PLAYER_H
#define PLAYER_H

#include <QVector2D>
#include <vssref_common.pb.h>

using VSSRef::Color;
class Player 

{

public:
    // How the fuck can we add skills in a composite manner? Extra class? Players are binded to skills, not the other way around. Kinda weird. 
    // VSSRef::Color can and SHOULD eventually be replaced with a generic enum to enhance portability.
    Player(Color color, quint8 id) : _playerColor(color), _playerId(id) 
    {}

    Color getPlayerColor();

    quint8 getPlayerId();

    float getOrientation();

    // to be implemented
    float getLinearSpeed();

    QVector2D getCoordinates();

    // to be implemented
    void goTo();
    
protected:

    friend class WorldMap;

    quint8 _playerId;
    Color _playerColor;
    float _vX, _vY;
    float _orientation;
    QVector2D _coordinates;

};

#endif //PLAYER_H