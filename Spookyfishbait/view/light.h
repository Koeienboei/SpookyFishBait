#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>
#include <QVector4D>

class Light
{
public:
    Light(QVector3D position, QVector4D color);

    QVector3D position;
    QVector4D color;
};

#endif // LIGHT_H
