#ifndef SKILLS_H
#define SKILLS_H

#include "player/player.h"
#include "actuator/actuator.h"
#include "vision/vision.h"
#include "basics/basic.h"

class Skills 

{

public:

    /* 
    How the fuck can we add skills to players in a composite manner? Extra class? Players are binded to skills, not the other way around. Kinda weird.
    Really don't want to reference an actuator when creating a player object. Makes no fucking sense.
    */  
   
    Skills(Player *currentPlayer, ActuatorClient *actuator) : _player(currentPlayer), _actuator(actuator) 
    {
        _actuator->setTeamColor(_player->getPlayerColor());
    };

    void goTo(QVector2D &targetCoordinates);

    void rotateTo(QVector2D &targetCoordinates);

    void spinKick();


private: 

    Player *_player;
    ActuatorClient *_actuator;

};




#endif