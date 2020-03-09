#ifndef PLAYER_H
#define PLAYER_H

class Player;

#include "controller.h"
#include "view/mainwindow.h"

#include <QString>

class Player : public Controller
{
public:
    Player(QString name, MainWindow *window);
    ~Player();

    // Controller functions
    bool w();
    bool a();
    bool s();
    bool d();
    bool space();

private:
    QString name;
    MainWindow *window;
};

#endif // PLAYER_H
