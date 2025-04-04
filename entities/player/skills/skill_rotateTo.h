#ifndef SKILL_ROTATETO
#define SKILL_ROTATETO

#include "skill_manager.h"

class RotateTo : public SkillManager 
{

static constexpr float KP = 15.0f; // How to consolidate all constants into a single header file?
static constexpr float KD = 2.5f;
static constexpr float BASE_SPEED = 30.0f;

public:
    
    explicit RotateTo(Player *player, float angle) : SkillManager(player), _angle(angle) 
    {}

    void runSkill(ActuatorClient *actuator) override;

    void rotateTo(float angle, ActuatorClient *actuator);

    float _angle;
};

#endif