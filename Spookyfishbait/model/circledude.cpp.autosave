#include "circledude.h"

// Math
#define abs(x) (x<0 ? -x : x)
#define min(a,b) ((a)<(b) ? a : b)
#define max(a,b) ((a)>(b) ? a : b)
#define PI 3.14159265359

// Axis
#define xAxis QVector3D(1.0,0.0,0.0)
#define yAxis QVector3D(0.0,1.0,0.0)
#define zAxis QVector3D(0.0,0.0,-1.0)



void CircleDude::step() {
    // Set new direction with walking buttons (wasd)
    calculateWalking();

    // Jump if on the ground and space is pressed
    calculateJumping();

    // Gravity
    calculateFalling();

    // Slide
    calculateSlide();

    // Calculate the new direction of facing
    calculateFacing();

    // Place the body on right position and with right rotation
    placeBody();

    // Place hands on right position
    placeHands();
}

void CircleDude::calculateWalking() {
    if (isOnGround() && !isSliding()) {
        // Move left
        if (controller->a())
            movementDirection.setX(max(-1, movementDirection.x() - 0.25));
        // Move right
        if (controller->d())
            movementDirection.setX(min(1, movementDirection.x() + 0.25));
        // If not moving left or right slowly come to stand still in x direction
        if (!controller->a() && !controller->d()) {
            if (movementDirection.x() < 0) {
                movementDirection.setX(min(0, movementDirection.x() + 0.25));
            } else {
                movementDirection.setX(max(0, movementDirection.x() - 0.25));
            }
        }
        // Move foward
        if (controller->w())
            movementDirection.setZ(max(-1, movementDirection.z() - 0.25));
        // Move backward
        if (controller->s())
            movementDirection.setZ(min(1, movementDirection.z() + 0.25));
        // If not moving foward or backward slowly come to stand still in z direction
        if (!controller->w() && !controller->s()) {
            if (movementDirection.z() < 0) {
                movementDirection.setZ(min(0, movementDirection.z() + 0.25));
            } else {
                movementDirection.setZ(max(0, movementDirection.z() - 0.25));
            }
        }
    }
    // Normalize movement direction if length in xz space is greater than 1
    QVector3D movementDirectionXY(movementDirection.x(), 0.0, movementDirection.z());
    if (movementDirectionXY.length()>1) {
        movementDirectionXY.normalize();
        movementDirection.setX(movementDirectionXY.x());
        movementDirection.setZ(movementDirectionXY.z());
    }
}

void CircleDude::calculateSlide() {
    // If in the air slide time is 15 otherwise it is reduced by 1
    if (getHeight() > 0.0) {
        slideTime = 15;
    } else {
        slideTime = max(0, slideTime-1);
        if (isSliding() && hasXZDirection()) {
            movementDirection = movementDirection.normalized() * (movementDirection.length()/1.2);
            body->getOBJModel()->setUsedTexture(1);
        } else {
            body->getOBJModel()->setUsedTexture(0);
        }
    }
}

void CircleDude::calculateJumping() {
    // Jump if on the ground and space is pressed
    if (controller->space() && isOnGround())
        movementDirection.setY(statistics->getJumpHeight());
}

void CircleDude::calculateFalling() {
    // Gravity
    movementDirection.setY(movementDirection.y() - 0.25);
}

void CircleDude::calculateFacing() {
    // If no movement in x and z facing remains the old facing
    if (hasXZDirection()) {
        facingDirection = QVector3D(movementDirection.x(), 0.0, movementDirection.z());
        facingDirection.normalize();
    }
}


void CircleDude::placeBody() {
    // Calculate new position of body
    QVector3D newBodyPosition = body->getPosition() + statistics->getMovementSpeed()*movementDirection;

    // If below ground, put on ground
    newBodyPosition.setY(max(0.0, newBodyPosition.y()));

    // Set new body position
    body->setPosition(newBodyPosition);

    // Rotate the body according to facing direction (use old if not moving in x or z)
    if (hasXZDirection()) {
        float zDotFacing = QVector3D::dotProduct(zAxis, facingDirection);
        float angle;
        if (movementDirection.x() > 0.0) {
            angle = -acos(zDotFacing) * 180/PI;
        } else {
            angle = acos(zDotFacing) * 180/PI;
        }
        body->setRotation(angle);
    }
}

void CircleDude::placeHands() {
    // Calculate new position of hands
    if (isOnGround() && !isSliding()) {
        calculatePositionHandsOpposide();
    } else {
        calculatePositionHandsSame();
    }

    // Set animation time and direction
    if (!hasXZDirection()) {                    // Standing, flying or sliding without direction
        calculateAnimationHandsMid(1);
    } else if (isOnGround() && !isSliding()){   // Walking
        calculateAnimationHandsWalking(1);
    } else if (!isOnGround()){                  // Flying
        calculateAnimationHandsFront(1);
    } else if (isSliding()) {                   // Sliding
        calculateAnimationHandsBack(2);
    }
}

void CircleDude::calculateAnimationHandsFront(int speed) {
    if (handAnimationTime < 4)
        handAnimationTime = min(3,handAnimationTime + speed);
}

void CircleDude::calculateAnimationHandsMid(int speed){
    if (handAnimationTime < 0) {
        handAnimationTime = min(0, handAnimationTime + speed);
    } else if (handAnimationTime > 0) {
        handAnimationTime = max(0, handAnimationTime - speed);
    }
}

void CircleDude::calculateAnimationHandsBack(int speed) {
    if (handAnimationTime > -4)
        handAnimationTime = max(-3,handAnimationTime - speed);
}

void CircleDude::calculateAnimationHandsWalking(int speed){
    if (handAnimationTime > 4)
        handAnimationDirection = -1;
    if (handAnimationTime < -4)
        handAnimationDirection = 1;
    handAnimationTime = handAnimationTime + handAnimationDirection*speed;
}

void CircleDude::calculatePositionHandsOpposide() {
    calculatePositionHands(-1);
}

void CircleDude::calculatePositionHandsSame(){
    calculatePositionHands(1);
}

void CircleDude::calculatePositionHands(int type) {
    // Vector right perpendicular to facing and yAxis
    QVector3D right = QVector3D::crossProduct(facingDirection, yAxis);

    // Calculate new position for hands
    QVector3D positionLeftHand(body->getPosition() - right*50 - right*cos(PI/10*handAnimationTime)*15 + type*facingDirection*handAnimationTime*10);
    QVector3D positionRightHand(body->getPosition() + right*50 + right*cos(PI/10*handAnimationTime)*15 + facingDirection*handAnimationTime*10);

    // Update hand directions
    leftHandDirection = QVector3D(positionLeftHand - leftHand->getPosition());
    rightHandDirection = QVector3D(positionRightHand - rightHand->getPosition());

    // Length of direction is max 25
    if (leftHandDirection.length()>25)
        leftHandDirection = leftHandDirection.normalized() * 25;
    if (rightHandDirection.length()>25)
        rightHandDirection = rightHandDirection.normalized() * 25;

    // Update hand positions
    leftHand->setPosition(leftHand->getPosition() + leftHandDirection * statistics->getMovementSpeed() / 10);
    rightHand->setPosition(rightHand->getPosition() + rightHandDirection * statistics->getMovementSpeed() / 10);
}



