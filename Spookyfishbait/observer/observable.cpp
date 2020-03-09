#include "observable.h"

Observable::Observable() {}
Observable::~Observable() {}

void Observable::attach (Observer *observer)
{
    observers.append(observer);
}

void Observable::detach(Observer *observer)
{
    observers.removeAll(observer);
}

void Observable::notifyObservers()
{
    for (int i= 0; i< observers.size(); i++)
    {
        observers[i]->update();
    }
}

