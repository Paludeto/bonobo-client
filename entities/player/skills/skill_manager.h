#ifndef SKILLS_H
#define SKILLS_H

#include "basics/basic.h"

// Template design pattern

class Player; // player forward decl.
class SkillManager

{

public:
   
    explicit SkillManager(Player *player) : _player(player)
    {};

    virtual ~SkillManager() = default;

    virtual void runSkill() = 0;


protected: 

    Player *_player;

};




#endif