#include "model.h"


Model::Model() {
    //Initialize lights
    for (int i=0; i<MAX_LIGHTS; i++) {
        lightColors[i] = QVector4D(0.0,0.0,0.0,0.0);
    }
}

Model::~Model() {
    //Delete and free all drawObjects
    for (int i=0; i<drawObjectsToRender.size(); i++) {
        delete drawObjectsToRender.at(i);
        free(drawObjectsToRender.at(i));
    }
}

void Model::step() {
    QVector<Unit*>::iterator iu;
    for (iu=units.begin(); iu!=units.end(); iu++) {
        (*iu)->step();
    }

    // Notify the main window
    notifyObservers();
}

void Model::addDrawObjectToInitialize(DrawObject *drawObject) {
    drawObjectsToInitialize.append(drawObject);
}

void Model::addDrawObjectToRender(DrawObject *drawObject) {
    drawObjectsToRender.append(drawObject);
    notifyObservers();
}

void Model::addUnit(Unit *unit) {
    units.append(unit);
    addDrawObjectToInitialize(unit->getBody());
    addDrawObjectToInitialize(unit->getLeftHand());
    addDrawObjectToInitialize(unit->getRightHand());
}

void Model::addPlayer(Player *player){
    players.append(player);
}

void Model::addLight(QVector3D position, QVector4D color) {
    int i;
    for (i=0; i<MAX_LIGHTS; i++) {
        if (lightColors[i]==QVector4D(0.0,0.0,0.0,0.0)) {
            lightPositions[i] = position;
            lightColors[i] = color;
            nLights++;
            notifyObservers();
            break;
        }
    }
    if (i==MAX_LIGHTS)
        qDebug() << "Max lights in scene, can't add more";
}

void Model::removeDrawObjectToRender(DrawObject *drawObject) {
    drawObjectsToRender.removeAll(drawObject);
    notifyObservers();
}

void Model::removeUnit(Unit *unit) {
    units.removeAll(unit);
    removeDrawObjectToRender(unit->getBody());
    removeDrawObjectToRender(unit->getLeftHand());
    removeDrawObjectToRender(unit->getRightHand());
}

void Model::removePlayer(Player *player) {
    players.removeAll(player);
}

void Model::removeLight(QVector3D position, QVector4D color) {
    for (int i=0; i<MAX_LIGHTS; i++) {
        if (lightColors[i]==color && lightPositions[i]==position) {
            lightColors[i] = QVector4D(0.0,0.0,0.0,0.0);
            nLights--;
            notifyObservers();
        }
    }
}
