#-------------------------------------------------
#
# Project created by QtCreator 2017-06-14T09:51:43
#
#-------------------------------------------------

QT       += core gui multimedia quickwidgets qml quickcontrols2

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


#For some unknown reason, built-in qmake variables are not visible, so they are read using the qmake command
QT_INSTALL_QML_PATH = $$system(qmake -query QT_INSTALL_QML)
message("QT_INSTALL_QML: $$QT_INSTALL_QML_PATH")

win32 {
    DESTDIR = C:/TODO/bin

    message("$$DESTDIR")
    message("$$OUT_PWD")
    message("$$TARGET")
    message("$$QT_MAJOR_VERSION")
    message("$$QT_MINOR_VERSION")
    message("$$QT_PATCH_VERSION")
    message("$$QT_VERSION")

    copy_files.commands = copy /y $$OUT_PWD\$${TARGET} $${DESTDIR}
    QMAKE_POST_LINK = windeployqt --qmldir $$QT_INSTALL_QML_PATH $$shell_path($$DESTDIR/$${TARGET}.exe)
}
