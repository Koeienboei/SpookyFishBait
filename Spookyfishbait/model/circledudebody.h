#ifndef CIRCLEDUDEBODY_H
#define CIRCLEDUDEBODY_H

#include "view/drawobject.h"

#include <QVector3D>
#include <QVector4D>

class CircleDudeBody : public DrawObject {

public:

    CircleDudeBody(QVector3D position) :
        DrawObject(position, 50.0, 0.0, QVector4D(0.3,0.8,0.0,0), ":/objects/circle.obj", {":/objects/circledudebody.png", ":/objects/circledudebodySlide.png"}) {
    }
};

#endif // CIRCLEDUDEBODY_H
