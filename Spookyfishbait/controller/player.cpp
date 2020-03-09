#include "player.h"

Player::Player(QString name, MainWindow *window) {
    this->name = name;
    this->window = window;
}

Player::~Player() {}

bool Player::w() {return window->w();}
bool Player::a() {return window->a();}
bool Player::s() {return window->s();}
bool Player::d() {return window->d();}
bool Player::space() {return window->space();}
