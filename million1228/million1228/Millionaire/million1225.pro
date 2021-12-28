#-------------------------------------------------
#
# Project created by QtCreator 2021-12-14T12:01:58
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Millionaire
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    bankfinal.cpp \
    improve_status.cpp \
        main.cpp \
        base_window.cpp \
    game_status.cpp \
    good.cpp \
    my_event.cpp \
    voiceres.cpp \
    job.cpp

HEADERS += \
    bankfinal.h \
        base_window.h \
    game_status.h \
    good.h \
    improve_status.h \
    my_event.h \
    voiceres.h \
    job.h

FORMS += \
        base_window.ui

RESOURCES += \
    res.qrc \
    resVoice.qrc

DISTFILES += \
    img/voiceRes/BackButtonSound.wav \
    img/voiceRes/ConFlipSound.wav \
    img/voiceRes/LevelWinSound.wav \
    img/voiceRes/TapButtonSound.wav
