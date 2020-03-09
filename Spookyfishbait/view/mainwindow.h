#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class MainWindow;

// Header files
#include "openglwindow.h"
#include "model/model.h"
#include "drawobject.h"

// Math includes
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix3x3>
#include <QMatrix4x4>

// ShaderProgram
#include <QOpenGLShaderProgram>

// Key and mouse events
#include <QKeyEvent>
#include <QMouseEvent>

// Timer
#include <QTimer>

class MainWindow : public OpenGLWindow , public Observer
{
public:
    MainWindow(Model *model);
    ~MainWindow();
    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

    // Update function for when observables are changed
    void update();

    bool w() {return wIsPressed;}
    bool a() {return aIsPressed;}
    bool s() {return sIsPressed;}
    bool d() {return dIsPressed;}
    bool space() {return spaceIsPressed;}

public slots:
    void updateFrame();

private:
    // DrawObject functions
    void initializeDrawObject(DrawObject *drawObject);
    void renderDrawObject(DrawObject *object);

    // Functions for keyboard input events
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

    // Model to be drawn
    Model *model;

    // Timer for updating screen
    QTimer *timer;

    // Shaderprogram
    QOpenGLShaderProgram *m_shaderProgram;

    // Transform matrices for model, view, projection and normal
    QMatrix4x4 modelMatrix;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projectionMatrix;
    QMatrix3x3 normalMatrix;

    // Uniform variables for matrices
    GLuint uModelMatrix;
    GLuint uViewMatrix;
    GLuint uProjectionMatrix;
    GLuint uNormalMatrix;

    // Vectors for rotation and translation of model and view
    QVector3D modelRotation;
    QVector3D modelTranslation;
    QVector3D viewRotation;
    QVector3D viewTranslation;

    //Uniformvariables for lights
    GLuint uLightPositions;
    GLuint uLightColors;
    GLuint uLightsSize;

    // Buttons
    bool wIsPressed;
    bool aIsPressed;
    bool sIsPressed;
    bool dIsPressed;
    bool spaceIsPressed;
};

#endif // MAINWINDOW_H
