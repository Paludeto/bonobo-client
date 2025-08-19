#ifndef SKILL_WALL_SPIN_H
#define SKILL_WALL_SPIN_H

#include "skill_manager.h"
#include <QElapsedTimer>

class WallSpinSkill : public SkillManager {
public:
    explicit WallSpinSkill(Player *player, bool spinClockwise);

    void runSkill(ActuatorClient *actuator) override;
    bool isComplete() const;

private:
    bool _spinClockwise;
    bool _isComplete;
    QElapsedTimer _spinTimer;
    static constexpr int SPIN_DURATION_MS = 500; 
    static constexpr float SPIN_SPEED = 100.0f;    
};

#endif // SKILL_WALL_SPIN_H