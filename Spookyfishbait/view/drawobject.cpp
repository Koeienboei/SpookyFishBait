#include "drawobject.h"

#include <qDebug>

DrawObject::DrawObject(QVector3D position, float scale, float rotation, QVector4D material, QString filepathModel, QVector<QString> filepathTextures)
{
    // Initialize position, scale, rotation and material
    this->position = position;
    this->scale = scale;
    this->rotation = rotation;
    this->material = material;

    // Object model
    objectModel = new OBJModel(filepathModel, filepathTextures);
}

DrawObject::~DrawObject() {
    delete objectModel;
    free(objectModel);
}

