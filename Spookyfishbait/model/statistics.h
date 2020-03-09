#ifndef STATISTICS_H
#define STATISTICS_H

class Statistics {

public:
    Statistics(double movementSpeed, double jumpHeight) {
        this->movementSpeed = movementSpeed;
        this->jumpHeight = jumpHeight;
    }

    double getMovementSpeed() {return movementSpeed;}
    double getJumpHeight() {return jumpHeight;}

private:

    double movementSpeed;
    double jumpHeight;

};

#endif // STATISTICS_H
