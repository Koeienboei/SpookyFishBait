#ifndef MODEL_H
#define MODEL_H

class Model;

#include "view/drawobject.h"
#include "unit.h"
#include "controller/player.h"
#include "light.h"

#include "observer/observable.h"
#include "observer/observer.h"

#include <QVector>
#include <QVector3D>
#include <QVector4D>

// Maximum number of lights that can be in a scene at once
#define MAX_LIGHTS 25

class Model : public Observable
{
public:
    Model();
    ~Model();  

    void step();

    void addDrawObjectToInitialize(DrawObject *drawObject);
    void addDrawObjectToRender(DrawObject *drawObject);
    void addUnit(Unit *unit);
    void addPlayer(Player *player);
    void addLight(QVector3D position, QVector4D color);

    void removeDrawObjectToRender(DrawObject *drawObject);
    void removeUnit(Unit *unit);
    void removePlayer(Player *player);
    void removeLight(QVector3D position, QVector4D color);

    DrawObject* popDrawObjectToInitialize()
    {
        DrawObject *pop = drawObjectsToInitialize.front();
        drawObjectsToInitialize.pop_front();
        return pop;
    }
    int drawObjectsToInitializeSize() {return drawObjectsToInitialize.size();}
    int drawObjectsToRenderSize() {return drawObjectsToRender.size();}
    QVector<DrawObject*> getDrawObjectsToRender() {return drawObjectsToRender;}
    QVector3D* getLightPositions() {return lightPositions;}
    QVector4D* getLightColors() {return lightColors;}
    int getLightsSize() {return nLights;}

private:
    QVector<DrawObject*> drawObjectsToInitialize;
    QVector<DrawObject*> drawObjectsToRender;
    QVector<Unit*> units;
    QVector<Player*> players;

    QVector3D lightPositions[MAX_LIGHTS];
    QVector4D lightColors[MAX_LIGHTS];
    int nLights = 0;
};

#endif // MODEL_H
