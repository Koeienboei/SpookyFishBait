#include "mainwindow.h"
#include "drawobject.h"

#include <QDebug>
#include <QScreen>

#define FARPLANE 10000.0
#define NEARPLANE 0.01
#define min(a,b) a<b ? a : b

MainWindow::MainWindow(Model *model) : m_shaderProgram(0)
{
    this->model = model;
    // Set MainWindow to be observer of model
    model->attach(this);

    // Buttons
    wIsPressed = false;
    aIsPressed = false;
    sIsPressed = false;
    dIsPressed = false;
    spaceIsPressed = false;
}

MainWindow::~MainWindow()
{
    // free the pointer of the shading program
    delete m_shaderProgram;
}

/*
 * Initialize window
 */
void MainWindow::initialize()
{
    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << "Using OpenGL" << qPrintable(glVersion);

    // Initialize the shaders
    m_shaderProgram = new QOpenGLShaderProgram(this);
    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex.glsl");
    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment.glsl");
    m_shaderProgram->link();

    // Set OpenGL to use Filled triangles (can be used to render points, or lines)
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Enable Z-buffering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    qDebug() << "Depth Buffer size:" <<  this->format().depthBufferSize() << "bits";

    // Set the clear color to be black (color used for resetting the screen)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Set intiatal view settings
    viewRotation = QVector3D(90.0, 0.0, 0.0);
    viewTranslation = QVector3D(0.0, 0.0, -500.0);

    // Set timer
    timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
    int msec = 25;
    timer->setInterval(msec);
    timer->start();
}

/*
 *  The render function, called when an update is requested
 */
void MainWindow::render()
{
    // X, y and z axis
    QVector3D X(1.0, 0.0, 0.0);
    QVector3D Y(0.0, 1.0, 0.0);
    QVector3D Z(0.0, 0.0, 1.0);

    // Translate and rotate view to right position
    viewMatrix.setToIdentity();
    viewMatrix.translate(viewTranslation);
    viewMatrix.rotate(viewRotation[0], X);
    viewMatrix.rotate(viewRotation[1], Y);
    viewMatrix.rotate(viewRotation[2], Z);

    // Set projection
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(60, (float)width()/height(), NEARPLANE, FARPLANE);

    // Bind the shaderprogram to use it
    m_shaderProgram->bind();

    // Set uniform variables for view matrix
    uViewMatrix = m_shaderProgram->uniformLocation("viewMatrix");
    m_shaderProgram->setUniformValue(uViewMatrix, viewMatrix);

    // Set uniform variables for projection matrix
    uProjectionMatrix = m_shaderProgram->uniformLocation("projectionMatrix");
    m_shaderProgram->setUniformValue(uProjectionMatrix, projectionMatrix);

    // Set uniform variables for lights
    uLightsSize = m_shaderProgram->uniformLocation("nLights");
    m_shaderProgram->setUniformValue(uLightsSize, model->getLightsSize());
    uLightPositions = m_shaderProgram->uniformLocation("lightPositions");
    m_shaderProgram->setUniformValueArray(uLightPositions, model->getLightPositions(), model->getLightsSize());
    uLightColors = m_shaderProgram->uniformLocation("lightColors");
    m_shaderProgram->setUniformValueArray(uLightColors, model->getLightColors(), model->getLightsSize());

    // glViewport is used for specifying the resolution to render
    // Uses the window size as the resolution
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    // Clear the screen at the start of the rendering.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render all the objects in the model
    for (int i=0; i<model->getDrawObjectsToRender().size(); i++) {
        renderDrawObject(model->getDrawObjectsToRender().at(i));
    }

    // Release shaderprogram
    m_shaderProgram->release();
}

/*
 * Initialize object
 */
void MainWindow::initializeDrawObject(DrawObject *drawObject)
{
    // Bind vertex array object
    drawObject->getOBJModel()->createVAO();
    drawObject->getOBJModel()->bindVAO();

    // Coordinates vertices (Location 0)
    drawObject->getOBJModel()->setCoordinatesBuffer(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer));
    drawObject->getOBJModel()->getCoordinatesBuffer()->create();
    drawObject->getOBJModel()->getCoordinatesBuffer()->bind();
    drawObject->getOBJModel()->getCoordinatesBuffer()->allocate(drawObject->getOBJModel()->getVertices().data(), drawObject->getOBJModel()->getVecticesSize()*sizeof(QVector3D));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Coordinates normals (Location 1)
    drawObject->getOBJModel()->setNormalsBuffer(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer));
    drawObject->getOBJModel()->getNormalsBuffer()->create();
    drawObject->getOBJModel()->getNormalsBuffer()->bind();
    drawObject->getOBJModel()->getNormalsBuffer()->allocate(drawObject->getOBJModel()->getNormals().data(), drawObject->getOBJModel()->getNormalsSize()*sizeof(QVector3D));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // Initialize textures
    for (int i=0; i<drawObject->getOBJModel()->getNumTextures(); i++) {
        // Coordinates texture (Location 2)
        drawObject->getOBJModel()->addTextureBuffer(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer));
        drawObject->getOBJModel()->getTextureBuffer(i)->create();
        drawObject->getOBJModel()->getTextureBuffer(i)->bind();
        drawObject->getOBJModel()->getTextureBuffer(i)->allocate(drawObject->getOBJModel()->getTexcoords().data(), drawObject->getOBJModel()->getTexcoordsSize()*sizeof(QVector2D));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);

        // Texture
        drawObject->getOBJModel()->addTexture(new QOpenGLTexture(QOpenGLTexture::Target2D));
        drawObject->getOBJModel()->getTexture(i)->setWrapMode(QOpenGLTexture::ClampToEdge);
        drawObject->getOBJModel()->getTexture(i)->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
        drawObject->getOBJModel()->getTexture(i)->setData(QImage(QString(drawObject->getOBJModel()->getFilepathTexture(i))).mirrored());
    }

    // Release vertex array object
    drawObject->getOBJModel()->releaseVAO();
}

/*
 * Render object
 */
void MainWindow::renderDrawObject(DrawObject *drawObject)
{
    // Translate model to right position
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.translate(drawObject->getPosition());
    modelMatrix.scale(drawObject->getScale());
    modelMatrix.rotate(drawObject->getRotation(), QVector3D(0.0,1.0,0.0));

    // Bind shaderprogram
    //m_shaderProgram->bind();

    // Set uniform variables
    GLuint uModelMatrix = m_shaderProgram->uniformLocation("modelMatrix");
    m_shaderProgram->setUniformValue(uModelMatrix, modelMatrix);
    GLuint uMaterial = m_shaderProgram->uniformLocation("material");
    m_shaderProgram->setUniformValue(uMaterial, drawObject->getMaterial());

    // Bind object
    drawObject->getOBJModel()->bindVAO();

    // Bind texture
    drawObject->getOBJModel()->getTexture(drawObject->getOBJModel()->getUsedTexture())->bind(GL_TEXTURE0);

    // Draw object
    glDrawArrays(GL_TRIANGLES, 0, drawObject->getOBJModel()->getVecticesSize());

    // Release object
    drawObject->getOBJModel()->releaseVAO();
}

void MainWindow::update() {
    if (m_context) {
        // Initialize drawObjects
        while(model->drawObjectsToInitializeSize()!=0) {
            DrawObject *drawObject = model->popDrawObjectToInitialize();
            initializeDrawObject(drawObject);
            model->addDrawObjectToRender(drawObject);
        }
    }
    renderLater();
}

void MainWindow::updateFrame() {
    model->step();
}

// Triggered by pressing a key
void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    switch(ev->key()) {
    case Qt::Key_W:
        wIsPressed = true;
        break;
    case Qt::Key_A:
        aIsPressed = true;
        break;
    case Qt::Key_S:
        sIsPressed = true;
        break;
    case Qt::Key_D:
        dIsPressed = true;
        break;
    case Qt::Key_Space:
        spaceIsPressed = true;
        break;
    }
}

// Triggered by releasing a key
void MainWindow::keyReleaseEvent(QKeyEvent *ev)
{
    switch(ev->key()) {
    case Qt::Key_W:
        wIsPressed = false;
        break;
    case Qt::Key_A:
        aIsPressed = false;
        break;
    case Qt::Key_S:
        sIsPressed = false;
        break;
    case Qt::Key_D:
        dIsPressed = false;
        break;
    case Qt::Key_Space:
        spaceIsPressed = false;
        break;
    }
}

