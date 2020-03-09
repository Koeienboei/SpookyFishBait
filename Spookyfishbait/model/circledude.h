#ifndef CIRCLEDUDE_H
#define CIRCLEDUDE_H

class CircleDude;

#include "unit.h"
#include "controller/controller.h"
#include "circledudebody.h"
#include "circledudehand.h"

#include <QString>
#include <QVector3D>
#include <QtMath>

class CircleDude : public Unit {

public:
    CircleDude(QString name, Controller *controller, QVector3D position) :
        Unit(name,
             controller,
             new Statistics(10.0, 3.5),
             new CircleDudeBody(position),
             new CircleDudeHand(QVector3D(position.x()+55.0, position.y(), position.z())),
             new CircleDudeHand(QVector3D(position.x()-55.0, position.y(), position.z())))
    {}

    void step();

private:
    void calculateWalking();
    void calculateJumping();
    void calculateFalling();
    void calculateFacing();
    void calculateSlide();
    void calculateAnimationHandsFront(int speed);
    void calculateAnimationHandsMid(int speed);
    void calculateAnimationHandsBack(int speed);
    void calculateAnimationHandsWalking(int speed);
    void calculatePositionHandsOpposide();
    void calculatePositionHandsSame();
    void calculatePositionHands(int type);
    void placeBody();
    void placeHands();

};

#endif // CIRCLEDUDE_H
