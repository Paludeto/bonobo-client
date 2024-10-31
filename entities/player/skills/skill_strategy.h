#ifndef SKILLS_H
#define SKILLS_H

#include "player/player.h"
#include "basics/basic.h"

// Terminar
class SkillStrategy

{

public:
   
    explicit SkillStrategy(Player *player) : _player(player)
    {};

    virtual ~SkillStrategy() = default;

    virtual void runSkill(QVector2D &targetCoordinates);


protected: 

    Player *_player;

};




#endif