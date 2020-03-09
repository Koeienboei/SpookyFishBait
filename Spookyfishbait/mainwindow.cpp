#include "mainwindow.h"

#include <QDebug>
#include <QScreen>
#include <string>

#define SPEEDUP 1
#define MAXVERTICALROTATION 90
#define PI 3.14159265358
#define FPS 60.0

MainWindow::MainWindow()
    : m_shaderProgram(0)
{
    qDebug() << "Constructor of MainWindow";

    // NOTE: OpenGL functions are not yet enabled when the constructor is called
}

MainWindow::~MainWindow()
{
    qDebug() << "Desctructor of MainWindow";

    // Free all your used resources here

    // Destroy buffers first, before destroying the VertexArrayObject
    object.destroy();
    coordinatesBuffer->destroy();
    textureBuffer->destroy();
    free(timer);
    free(coordinatesBuffer);
    free(textureBuffer);

    // free the pointer of the shading program
    delete m_shaderProgram;

}

// Initialize all your OpenGL objects here
void MainWindow::initialize()
{
    qDebug() << "MainWindow::initialize()";
    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << "Using OpenGL" << qPrintable(glVersion);

    timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
    int msec = 1000/60;
    qDebug() << "timer: " << msec;
    timer->setInterval(msec);
    timer->start();

    // Initialize the shaders
    m_shaderProgram = new QOpenGLShaderProgram(this);
    // Use the ":" to load from the resources files (i.e. from the resources.qrc)
    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex.glsl");
    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment.glsl");
    m_shaderProgram->link();

    // Shaders are initialized
    // You can retrieve the locations of the uniforms from here


    // Initialize your objects and buffers

    OBJModel cube = OBJModel(":/models/sphere.obj");

    // Create your Vertex Array Object (VAO) and Vertex Buffer Objects (VBO) here.
    QVector<QVector3D> vertices = cube.vertices;
    nVertices = vertices.size();

    QVector<QVector2D> textureCoordinates = cube.texcoords;
    nTexCoor = textureCoordinates.size();

    // VAO
    object.create();
    object.bind();

    // Coordinates vertices
    coordinatesBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    coordinatesBuffer->create();
    coordinatesBuffer->bind();
    coordinatesBuffer->allocate(vertices.data(), nVertices*sizeof(QVector3D));
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Coordinates texture
    textureBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    textureBuffer->create();
    textureBuffer->bind();
    textureBuffer->allocate(textureCoordinates.data(), nTexCoor*sizeof(QVector2D));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    for (int i=0; i<10; i++) {
        textures.append(new QOpenGLTexture(QOpenGLTexture::Target2D));
        textures.at(i)->setWrapMode(QOpenGLTexture::ClampToEdge);
        textures.at(i)->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
        //textures.at(i)->setData(QImage(QString(":/models/textures/").append(fileNames[i])).mirrored());
        textures.at(i)->setData(QImage(QString(":/models/textures/testgrid.png")).mirrored());
    }

    object.release();

    // Set OpenGL to use Filled triangles (can be used to render points, or lines)
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    // Enable Z-buffering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    qDebug() << "Depth Buffer size:" <<  this->format().depthBufferSize() << "bits";

    // Function for culling, removing faces which don't face the camera
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    // Set the clear color to be black (color used for resetting the screen)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    isPressed = false;

    modelRotation = QVector3D(0.0, 0.0, 0.0);
    modelTranslation = QVector3D(0.0, 0.0, 0.0);
    viewRotation = QVector3D(0.0, 0.0, 0.0);
    viewTranslation = QVector3D(0.0, 0.0, -200000.0);

    viewDirection = QVector3D(0.0, 0.0,1.0);
    viewUp = QVector3D(0, 1, 0.0);
    viewSpeed = 0.0;
}

void MainWindow::updateFrame() {

    double rotateUp = (width()/2 - lastY) / 250.0 / FPS;
    double rotateRight = (lastX - width()/2) / 250.0 / FPS;

    QVector3D viewRight = QVector3D::normal(viewDirection, viewUp);

    QVector3D tmp = viewDirection * cos(rotateUp) + viewUp * sin(rotateUp);
    viewUp = viewUp * cos(rotateUp) - viewDirection * sin(rotateUp);
    viewDirection = tmp * cos(rotateRight) + viewRight * sin(rotateRight);


    viewDirection.normalize();
    viewUp.normalize();

    for (int i=0; i<10; i++) {
        selfRotationDegree[i] += 360.0/selfRotationTime[i]/FPS;
        sunRotationDegree[i] += 360.0/sunRotationTime[i]/FPS;
    }
    viewTranslation += viewDirection*viewSpeed/FPS;
    renderLater();
}

void MainWindow::renderSphere(QVector3D pos, QVector4D material, QOpenGLTexture *texture, double size, double rotation)
{
    modelTransformMatrix.setToIdentity();

    if (pos.length() > 0.0) {
        modelTransformMatrix.translate(pos);
        modelTransformMatrix.rotate(rotation, QVector3D(0,1,0));
    }
    modelTransformMatrix.scale(size);


    m_shaderProgram->bind();
    texture->bind(GL_TEXTURE0);

    GLuint modelUniformMatrix = m_shaderProgram->uniformLocation("modelMatrix");
    m_shaderProgram->setUniformValue(modelUniformMatrix, modelTransformMatrix);

    GLuint phongComponent = m_shaderProgram->uniformLocation("phongComponent");
    m_shaderProgram->setUniformValue(phongComponent, material);

    glDrawArrays(GL_TRIANGLES, 0, nVertices);
}

void MainWindow::renderRaytracerScene()
{
    for (int i=0; i<10; i++) {
        renderSphere(QVector3D(distances[i]*cos(sunRotationDegree[i]/180.0*PI), 0, distances[i]*sin(sunRotationDegree[i]/180.0*PI)), QVector4D(0.2f,0.7f,0.5f,64), textures.at(i), sizes[i], selfRotationDegree[i]);
    }
}

// The render function, called when an update is requested
void MainWindow::render()
{
    viewTransformMatrix.setToIdentity();

    QVector3D X(1.0, 0.0, 0.0);
    QVector3D Y(0.0, 1.0, 0.0);
    QVector3D Z(0.0, 0.0, 1.0);

    // Phi
    viewRotation[0] = acos(QVector3D::dotProduct(viewDirection, Y)) / PI * 180.0 - 90.0;

    // Theta
    QVector3D Ymap(viewDirection.x(), 0.0, viewDirection.z());
    Ymap.normalize();
    viewRotation[1] = acos(QVector3D::dotProduct(Ymap, Z)) / PI * 180.0;
    if (Ymap.x() > 0)
        viewRotation[1] = -viewRotation[1];

    /*/ Rotation
    QMatrix4x4 rot;
    rot.setToIdentity();
    rot.rotate(-viewRotation[0], QVector3D::crossProduct(Y, Ymap));
    rot.rotate(-viewRotation[1], Y);
    QVector4D rotUp4 = rot * QVector4D(viewUp);
    QVector3D rotUp = rotUp4.toVector3D();
    viewRotation[2] = acos(QVector3D::dotProduct(Y, rotUp)) / PI * 180.0;
    if (rotUp.x() < 0)
        viewRotation[2] = -viewRotation[2];

    qDebug() << "Direction vector + up vector: ";
    qDebug() << viewDirection << viewUp;
    qDebug() << Ymap << Y << QVector3D::crossProduct(Y, Ymap);
    qDebug() << rotUp << viewRotation;
    */

    viewTransformMatrix.rotate(viewRotation[0], X);
    viewTransformMatrix.rotate(viewRotation[1], Y);
    viewTransformMatrix.rotate(viewRotation[2], Z);
    viewTransformMatrix.translate(viewTranslation);

    projectionTransformMatrix.setToIdentity();
    projectionTransformMatrix.perspective(60, (float)width()/height(), 100.0, 10000000.0);

    // Bind the shaderprogram to use it
    m_shaderProgram->bind();

    viewUniformMatrix = m_shaderProgram->uniformLocation("viewMatrix");
    projectionUniformMatrix = m_shaderProgram->uniformLocation("projectionMatrix");

    m_shaderProgram->setUniformValue(viewUniformMatrix, viewTransformMatrix);
    m_shaderProgram->setUniformValue(projectionUniformMatrix, projectionTransformMatrix);

    // glViewport is used for specifying the resolution to render
    // Uses the window size as the resolution
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    // Clear the screen at the start of the rendering.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // VAO
    object.bind();

    renderRaytracerScene();

    m_shaderProgram->release();
}


// Below are functions which are triggered by input events:

// Triggered by pressing a key
void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    Q_UNUSED(ev);
}

// Triggered by releasing a key
void MainWindow::keyReleaseEvent(QKeyEvent *ev)
{
    Q_UNUSED(ev);
}

// Triggered by clicking two subsequent times on any mouse button.
void MainWindow::mouseDoubleClickEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
}

// Triggered when moving the mouse inside the window (even when no mouse button is clicked!)
void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    lastX = ev->pos().x();
    lastY = ev->pos().y();
}

// Triggered when pressing any mouse button
void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
}

// Triggered when releasing any mouse button
void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
}

// Triggered when clicking scrolling with the scroll wheel on the mouse
void MainWindow::wheelEvent(QWheelEvent * ev)
{
    Q_UNUSED(ev);
}
