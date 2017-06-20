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


    timerZapisu = new QTimer();
    connect(timerZapisu, SIGNAL(timeout()),this, SLOT(timeoutTextChanged()));

    menu->addAction(hide_window);
    menu->addAction(restore);
    menu->addAction(quitAction);

    icon->setContextMenu(menu);

}

void Widget::setEditTabProperties(QTextEdit * textEdit){

}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    organizationName("Reyfel"),
    applicationName("TODO List"),
    notesContetKey("NotesContent")
{
    ui->setupUi(this);
    this->setWindowTitle(applicationName);
    setWindowFlags(/*Qt::FramelessWindowHint |*/ Qt::WindowStaysOnBottomHint);

    createMinimalizeToTry();

    readAndSetWindowGeometry();


    restoreNotes();

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

    if(newTabNameForm != nullptr){
        delete newTabNameForm;
    }
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

void Widget::onTextChanged(){
    qDebug() << "On text changed()" ;
    timerZapisu->start(10000); //zapisz zawartosc po 10sekundach od ostatniej zmiany
}

void Widget::timeoutTextChanged(){
    qDebug() << "Save notes timer()";
    timerZapisu->stop();
    saveNotes();
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

void Widget::restoreNotes(){
    QSettings settings(organizationName, applicationName);

    int notesCount = settings.value("notesCount").toInt();

    for(int i = 0; i < notesCount; i++){
        settings.beginGroup(QString::number(i));
        {
            this->insertNewTab(settings.value("noteName").toString(),settings.value("noteContent").toString(), i);
        }
        settings.endGroup();
    }
}

void Widget::saveNotes(){

    int notesCount = ui->tabWidget->count() - 1;// number of tabs minus "+" tab

    QSettings settings(organizationName, applicationName);
    settings.setValue("notesCount",notesCount);

    for(int i = 0; i < notesCount; i++){
        settings.beginGroup(QString::number(i));
        {
            settings.setValue("noteName", ui->tabWidget->tabText(i));
            settings.setValue("noteContent",(static_cast<QTextEdit *>(ui->tabWidget->widget(i)))->toPlainText());
        }
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

void Widget::onNewFormNameOKClicked(){
    QString newTabName = newTabNameForm->getText();
    this->insertNewTab(newTabName,"", ui->tabWidget->currentIndex());
}

void Widget::onNewFormNameCancelClicked(){

}

void Widget::insertNewTab(QString tabName, QString tabContent, int index){
    QTextEdit * textEdit = new QTextEdit();
    textEdit->setText(tabContent);
    ui->tabWidget->insertTab(index,textEdit,tabName);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->currentIndex()-1);

    connect(textEdit,SIGNAL(textChanged()),this,SLOT(onTextChanged()));
}

void Widget::on_tabWidget_tabBarClicked(int index){
    if("+" == ui->tabWidget->tabText(index)){
        if(newTabNameForm == nullptr){
            newTabNameForm = new NewTabNameForm();
            connect(newTabNameForm,SIGNAL(onOKClicked()),this,SLOT(onNewFormNameOKClicked()));
            connect(newTabNameForm,SIGNAL(onCancelClicked()),this,SLOT(onNewFormNameCancelClicked()));
        }
        newTabNameForm->show();

    }
}
