#ifndef CIRCLEDUDEHAND_H
#define CIRCLEDUDEHAND_H

#include "view/drawobject.h"

#include <QVector3D>
#include <QVector4D>

class CircleDudeHand : public DrawObject {

public:

    CircleDudeHand(QVector3D position) :
        DrawObject(position, 15.0, 0.0, QVector4D(0.3,0.8,0.0,0), ":/objects/circle.obj", {":/objects/circledudehand.png"}) {}
};

#endif // CIRCLEDUDEHAND_H
