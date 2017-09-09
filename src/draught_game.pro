#-------------------------------------------------
#
# Project created by QtCreator 2017-09-08T13:50:05
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = draught_game
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
        main.cpp \
        mainwindow.cpp \
    draughtboard.cpp \
    board.cpp

HEADERS += \
        mainwindow.h \
    draughtboard.h \
    board.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    img/blackK.png \
    img/blackP.png \
    img/empty.png \
    img/whiteK.png \
    img/whiteP.png \
    wav/Chatwheel_all_dead.wav \
    wav/Chatwheel_ay_ay_ay.wav \
    wav/Chatwheel_brutal.wav \
    wav/Chatwheel_charge.wav \
    wav/Chatwheel_crash_burn.wav \
    wav/Chatwheel_cricket.wav \
    wav/Chatwheel_crybaby.wav \
    wav/Chatwheel_disastah.wav \
    wav/Chatwheel_drum_roll.wav \
    wav/Chatwheel_ehto_g_g.wav \
    wav/Chatwheel_frog.wav \
    wav/Chatwheel_headshake.wav \
    wav/Chatwheel_rimshot.wav \
    wav/Chatwheel_wan_bu_liao_la.wav \
    wav/Chatwheel_wow.wav \
    wav/Chatwheel_zou_hao_bu_song.wav \
    wav/Crowd_lv_01.wav \
    wav/Crowd_lv_02.wav \
    wav/Chatwheel_sad_bone.wav
