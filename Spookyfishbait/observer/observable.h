#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include "observer.h"
#include <QVector>

class Observable
{
public:
    Observable();
    ~Observable();

    void attach(Observer *observer);
    void detach(Observer *observer);
    void notifyObservers();

private:
    QVector<Observer*> observers;
};

#endif // OBSERVABLE_H
