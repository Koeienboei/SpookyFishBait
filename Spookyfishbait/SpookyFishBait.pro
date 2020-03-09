#-------------------------------------------------
#
# Project created by QtCreator 2016-03-22T18:12:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpookyFishBait
TEMPLATE = app

SOURCES += \
    main.cpp \
    spookyfishbait.cpp \
    model/model.cpp \
    model/circledude.cpp \
    view/openglwindow.cpp \
    view/mainwindow.cpp \
    view/drawobject.cpp \
    view/objmodel.cpp \
    controller/controller.cpp \
    controller/player.cpp \
    observer/observable.cpp \
    observer/observer.cpp

HEADERS  += \
    spookyfishbait.h \
    model/model.h \
    model/unit.h \
    model/statistics.h \
    model/circledude.h \
    model/circledudehand.h \
    model/circledudebody.h \
    view/openglwindow.h \
    view/mainwindow.h \
    view/drawobject.h \
    view/objmodel.h \
    view/vertex.h \
    controller/controller.h \
    controller/player.h \
    observer/observable.h \
    observer/observer.h

FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += \
    resources.qrc

LIBS += -lopengl32

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../opengl/GL Files/lib/' -lOpenGL32
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../opengl/GL Files/lib/' -lOpenGL32
#else:unix: LIBS += -L$$PWD/'../../opengl/GL Files/lib/' -lOpenGL32

#INCLUDEPATH += $$PWD/'../../opengl/GL Files/lib'
#DEPENDPATH += $$PWD/'../../opengl/GL Files/lib'
