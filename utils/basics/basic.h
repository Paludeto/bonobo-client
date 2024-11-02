#ifndef BASIC_H
#define BASIC_H

#include <QVector2D>

class Basic 

{

public:

    static float getAngle(QVector2D currentPos, QVector2D &targetPos);

    static float getDistance(QVector2D currentPos, QVector2D &targetPos);

};


#endif // BASIC_H