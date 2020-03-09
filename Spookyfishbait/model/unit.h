#ifndef UNIT_H
#define UNIT_H

class unit;

#include "view/drawobject.h"
#include "controller/controller.h"
#include "statistics.h"

#include <QString>
#include <QVector3D>

class Unit
{
public:
    Unit(QString name, Controller *controller, Statistics *statistics, DrawObject *body, DrawObject *leftHand, DrawObject *rightHand) {
        this->name = name;
        this->controller = controller;
        this->statistics = statistics;
        movementDirection = QVector3D(0.0,0.0,0.0);
        facingDirection = QVector3D(0.0,0.0,-1.0);
        handAnimationDirection = 1;
        handAnimationTime = 0;
        slideTime = 0;
        this->body = body;
        this->leftHand = leftHand;
        this->rightHand = rightHand;
    }

    virtual void step() = 0;

    double getHeight() {return body->getPosition().y();}
    bool isOnGround() {return getHeight() == 0;}
    bool isSliding() {return slideTime>0 && slideTime<15;}
    bool hasXZDirection() {return movementDirection.x()!=0.0 || movementDirection.z()!=0.0;}

    DrawObject* getBody() {return body;}
    DrawObject* getLeftHand() {return leftHand;}
    DrawObject* getRightHand() {return rightHand;}

protected:
    // Name of the unit
    QString name;

    // Controller of the unit (Player or NPC)
    Controller *controller;

    // Stats of the unit
    Statistics *statistics;

    // Vectors for position and directions
    QVector3D movementDirection;
    QVector3D facingDirection;

    // Hands variables
    QVector3D leftHandDirection;
    QVector3D rightHandDirection;
    int handAnimationTime;
    int handAnimationDirection;

    // Slide
    int slideTime;

    // Drawobjects
    DrawObject *body;
    DrawObject *leftHand;
    DrawObject *rightHand;
};

#endif // UNIT_H
