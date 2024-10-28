#include "skills.h"

void Skills::goTo(QVector2D &targetPosition) {

    // Giving me reversed results in radians.
    float errorAngle = Basic::getAngle(_player->getCoordinates(), targetPosition);

    std::cout << "Error: " << errorAngle << std::endl;

}

// Finish