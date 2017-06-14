#include "widget.hpp"
#include "ui_widget.h"

#include <QPixmap>
#include <QPainter>
#include <QSettings>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

void Widget::createMinimalizeToTry(void)
{
    icon= new QSystemTrayIcon(this);
    QPixmap pixmap(25,25);
    pixmap.fill(Qt::green);
    QPainter painter(&pixmap);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10));
    painter.drawText(pixmap.rect(),Qt::AlignCenter,"TODO");
    icon->setIcon(pixmap);
    icon->setToolTip("TODO List, kliknij aby rozwinąć");
    icon->setVisible(true);
    icon->show();

    menu = new QMenu(this);


    hide_window = new QAction("Minimalizuj",this);
    connect( hide_window,SIGNAL(triggered()),this,SLOT(hide()));

    quitAction = new QAction("Zamknij",this);
    connect(quitAction,SIGNAL(triggered()),this,SLOT(on_close()));

    restore = new QAction("Przywróć", this);
    connect (restore, SIGNAL(triggered()), this, SLOT(showNormal()));


    menu->addAction(hide_window);
    menu->addAction(restore);
    menu->addAction(quitAction);

    icon->setContextMenu(menu);

}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    organizationName("Reyfel"),
    applicationName("TODO List")
{
    ui->setupUi(this);
    this->setWindowTitle(applicationName);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint);

    createMinimalizeToTry();

    readAndSetWindowGeometry();


    QFont font("Purisa", 10, QFont::Bold);

    ui->textEdit->setFont(font);

}

Widget::~Widget()
{


    saveNotes();

    saveWindowGeometry();

    menu->removeAction(quitAction);
    menu->removeAction(restore);
    menu->removeAction(hide_window);

    delete restore;
    delete quitAction;
    delete hide_window;
    delete menu;

    delete icon;
    delete ui;
}

void Widget::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}


void Widget::on_close()
{
    QApplication::quit();
}


void Widget::readAndSetWindowGeometry(){
    int windowHeight;
    int windowWidth;
    int windowXposition;
    int windowYposition;
    QSettings settings(organizationName, applicationName);
    windowHeight = settings.value("windowHeight").toInt();
    windowWidth = settings.value("windowWidth").toInt();
    windowXposition = settings.value("windowXposition").toInt();
    windowYposition = settings.value("windowYposition").toInt();

    if(windowHeight < 70){
        windowHeight = 320;
    }

    if(windowWidth < 70){
        windowWidth = 480;
    }


    this->setGeometry(windowXposition,windowYposition,windowWidth,windowHeight);
}


void Widget::saveNotes(){

    QSettings settings(organizationName, applicationName);

    for(int i = 0; i < ui->tabWidget->count(); i++){

        settings.beginGroup(ui->tabWidget->tabText(i));
        settings.setValue("Contet", ui->size());
        settings.endGroup();
    }

    settings.sync();

}

void Widget::saveWindowGeometry(){
    QSettings settings(organizationName, applicationName);
    settings.setValue("windowHeight",this->height());
    settings.setValue("windowWidth",this->width());
    settings.setValue("windowXposition",this->pos().x());
    settings.setValue("windowYposition",this->pos().y());
    settings.sync();
}
