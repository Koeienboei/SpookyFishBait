#include "spookyfishbait.h"

#define WIDTH 1000
#define HEIGHT 1000

SpookyFishBait::SpookyFishBait(QScreen *s)
{
    screen = s;
    initialize();
}

SpookyFishBait::~SpookyFishBait() {
    delete model;
    delete window;
    free(model);
    free(window);
    free (screen);
}

void SpookyFishBait::initialize() {
    // Create Model
    model = new Model();

    // Configure OpenGL to use the 3.3 core version
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3);

    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << "Using OpenGL" << qPrintable(glVersion);

    // Amount of bits used for the Z-buffer (24 usually)
    format.setDepthBufferSize(24);
    // Set super sampling
    format.setSamples(16);

    // Create MainWindow
    window = new MainWindow(model);
    window->setFormat(format);

    // Resize the window and place it on the center of the screen
    QRect size(screen->geometry());
    window->setGeometry(size.width()/2-(WIDTH/2),size.height()/2-(HEIGHT/2),WIDTH,HEIGHT);

    // Show MainWindow
    window->show();

    // Add white light at position (-200.0,-1500.0,600.0)
    model->addLight(QVector3D(0.0,150.0,0.0), QVector4D(1.0,1.0,1.0,1.0));

    // Add player
    Player *player = new Player("Koeienboei", window);

    // Add unit for player
    CircleDude *playerUnit = new CircleDude("Koeienboei", player, QVector3D(0.0,0.0,0.0));

    model->addPlayer(player);
    model->addUnit(playerUnit);
}
