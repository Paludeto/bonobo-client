#ifndef PLAYER_H
#define PLAYER_H

#include <QVector2D>
#include <vssref_common.pb.h>
#include "skills/skill_manager.h"
#include "actuator/actuator.h"

using VSSRef::Color;
class Player 

{

public:
    
    // VSSRef::Color can and SHOULD eventually be replaced with a generic enum to enhance portability.
    Player(Color color, quint8 id) : _playerColor(color), _playerId(id) 
    {}

    Color getPlayerColor();

    quint8 getPlayerId();

    void goTo(QVector2D &targetCoordinates, ActuatorClient *actuator);

    float getOrientation();

    float getLinearSpeed(); // Implement

    QVector2D getCoordinates();
    
protected:

    friend class WorldMap;

    std::unique_ptr<SkillManager> _skillManager;

    quint8 _playerId;
    Color _playerColor;
    float _vX, _vY;
    float _orientation;
    QVector2D _coordinates;

};

#endif //PLAYER_H