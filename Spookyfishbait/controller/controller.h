#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller;

#include <qDebug>

class Controller
{
public:
    Controller();
    ~Controller();

    virtual bool w() = 0;
    virtual bool a() = 0;
    virtual bool s() = 0;
    virtual bool d() = 0;
    virtual bool space() = 0;

private:

};

#endif // CONTROLLER_H
