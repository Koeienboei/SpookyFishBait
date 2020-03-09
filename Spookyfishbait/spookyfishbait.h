#ifndef SPOOKYFISHBAIT_H
#define SPOOKYFISHBAIT_H

#include "model/model.h"
#include "view/mainwindow.h"
#include "controller/player.h"
#include "model/unit.h"
#include "model/circledude.h"

#include <QGuiApplication>
#include <QScreen>

class SpookyFishBait
{
public:
    SpookyFishBait(QScreen *s);
    ~SpookyFishBait();

private:
    Model *model;
    MainWindow *window;
    QSurfaceFormat format;
    QScreen *screen;

    void initialize();
};

#endif // SPOOKYFISHBAIT_H
