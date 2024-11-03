#ifndef SKILL_GOTO
#define SKILL_GOTO

#include "skill_manager.h"

class GoTo : public SkillManager 
{

static constexpr float KP = 20; // How to consolidate all constants into a single header file?
static constexpr float KD = 2.5;
static constexpr float BASE_SPEED = 30.0f;

public:
    
    explicit GoTo(Player* player, QVector2D &targetCoordinates) : SkillManager(player), _targetCoordinates(targetCoordinates)
    {}

    void runSkill(ActuatorClient *actuator) override;

    void goTo(QVector2D &targetCoordinates, ActuatorClient *actuator);

    QVector2D &_targetCoordinates;

};

#endif