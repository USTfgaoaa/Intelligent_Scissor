#-------------------------------------------------
#
# Project created by QtCreator 2016-02-27T18:50:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = _Intelligent_Scissor
TEMPLATE = app


SOURCES += main.cpp \
    Intelligent_Scissor.cpp \
    function.cpp

HEADERS  += \
    Intelligent_Scissor.h \
    function.h

FORMS    += \
    Intelligent_Scissor.ui

INCLUDEPATH += /usr/include/opencv2
INCLUDEPATH += /usr/include/opencv
#INCLUDEPATH += /usr/include/eigen3

LIBS += -lopencv_core -lopencv_highgui -lopencv_ml -lopencv_imgproc
#LIBS += -I
