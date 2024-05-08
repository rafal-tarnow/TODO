#-------------------------------------------------
#
# Project created by QtCreator 2017-06-14T09:51:43
#
#-------------------------------------------------

QT       += core gui multimedia quickwidgets quickcontrols2

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TODO
TEMPLATE = app


SOURCES += main.cpp\
    settingsdialog.cpp \
        widget.cpp \
    newtabnameform.cpp

HEADERS  += widget.hpp \
    newtabnameform.hpp \
    settingsdialog.h

FORMS    += widget.ui \
    newtabnameform.ui \
    settingsdialog.ui

RESOURCES += \
    resources.qrc
    MenuBar.qml
    SettingsDialogQml.qml

