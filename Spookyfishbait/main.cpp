#include "view/mainwindow.h"
#include "view/drawobject.h"
#include "controller/player.h"
#include "model/light.h"
#include "spookyfishbait.h"

#include <QGuiApplication>
#include <QScreen>

#include <QDebug>

#define WIDTH 1000
#define HEIGHT 1000

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    SpookyFishBait *game = new SpookyFishBait(app.primaryScreen());

    return app.exec();
}
