#ifndef OBJECT_H
#define OBJECT_H

class DrawObject;

#include <QString>
#include <QVector3D>
#include <QVector4D>

#include "objmodel.h"
#include "observer/observable.h"

class DrawObject
{
public:
    DrawObject(QVector3D position, float scale, float rotation, QVector4D material, QString filepathModel, QVector<QString> filepathTexture);
    ~DrawObject();

    void setPosition(QVector3D p) {position = p;}
    void setScale(float s) {scale = s;}
    void setRotation(float r) {rotation = r;}
    void setMaterial(QVector4D m) {material = m;}
    void setOBJModel(OBJModel* om) {objectModel = om;}

    QVector3D getPosition() {return position;}
    float getScale() {return scale;}
    float getRotation() {return rotation;}
    QVector4D getMaterial() {return material;}
    OBJModel* getOBJModel() {return objectModel;}

protected:
    QVector3D position;
    float scale;
    float rotation;
    QVector4D material;
    OBJModel *objectModel;
};

#endif // OBJECT_H
