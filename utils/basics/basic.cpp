#include "basic.h"


float Basic::getAngle(QVector2D current, QVector2D &target) {

    return atan2(target.y() - current.y(), target.x() - current.x());

}