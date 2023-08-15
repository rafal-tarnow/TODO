#include "widget.hpp"
#include "ui_widget.h"

#include <QPixmap>
#include <QPainter>
#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <QApplication>
#include <QProcess>

#include <unistd.h>
#include <sys/socket.h>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    organizationName("Reyfel"),
    applicationName("TODO List"),
    notesContetKey("NotesContent")
{
    ui->setupUi(this);
    this->setWindowTitle(applicationName);
    this->setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(/*Qt::FramelessWindowHint |*/ Qt::WindowStaysOnBottomHint);

    createMinimalizeToTry();

    readAndSetWindowGeometry();
    restoreNotes();
    restoreTime();

    ui->tabWidget->setMovable(true);

    ui->tabWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tabWidget,SIGNAL(customContextMenuRequested(QPoint )),this , SLOT(showTabContextMenu(QPoint )));
    //connect(ui->tabWidget, SIGNAL(tabBarClicked(int)),this, SLOT(onTabContextMenuRequest(int)));


    timerLabelRefresh = new QTimer();
    connect(timerLabelRefresh, SIGNAL(timeout()), this, SLOT(timerLabelRefreshSlot()));
    timerLabelRefresh->start(1000);

    totalTimeSaveTimer = new QTimer();
    connect(totalTimeSaveTimer, SIGNAL(timeout()), this, SLOT(saveTime()));
    totalTimeSaveTimer->start(5*60*1000); //5min


    startTimeString = QDateTime().currentDateTime().toString("hh:mm:ss  dd/MM/yyyy");
    currentTimeString = QDateTime().currentDateTime().toString("hh:mm:ss  dd/MM/yyyy");
    diffTimeString = QString("%1").arg(QDateTime().fromString(startTimeString ,"hh:mm:ss  dd/MM/yyyy").msecsTo(QDateTime().fromString(currentTimeString ,"hh:mm:ss AP dd/MM/yyyy")));

    ui->labelStartTime->setText(startTimeString);
    ui->labelDiffTime->setText(diffTimeString);

    connect(qApp, SIGNAL(commitDataRequest(QSessionManager & )), this, SLOT(saveTime()));

    //HANDLE UNIX SIGNALS IN QT
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sighupFd))
        qFatal("Couldn't create HUP socketpair");

    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigtermFd))
        qFatal("Couldn't create TERM socketpair");
    snHup = new QSocketNotifier(sighupFd[1], QSocketNotifier::Read, this);
    connect(snHup, SIGNAL(activated(QSocketDescriptor)), this, SLOT(handleSigHup()));
    snTerm = new QSocketNotifier(sigtermFd[1], QSocketNotifier::Read, this);
    connect(snTerm, SIGNAL(activated(QSocketDescriptor)), this, SLOT(handleSigTerm()));
}


int Widget::sighupFd[2];
int Widget::sigtermFd[2];

void Widget::hupSignalHandler(int)
{
    char a = 1;
    ::write(sighupFd[0], &a, sizeof(a));
}

void Widget::termSignalHandler(int)
{
    char a = 1;
    ::write(sigtermFd[0], &a, sizeof(a));
}

void Widget::handleSigTerm()
{
    snTerm->setEnabled(false);
    char tmp;
    ::read(sigtermFd[1], &tmp, sizeof(tmp));

    // do Qt stuff
    QSettings settings(organizationName, applicationName);
    settings.setValue("SIGNAL TERM", true);
    settings.sync();

    snTerm->setEnabled(true);
}

void Widget::handleSigHup()
{
    snHup->setEnabled(false);
    char tmp;
    ::read(sighupFd[1], &tmp, sizeof(tmp));

    // do Qt stuff
    QSettings settings(organizationName, applicationName);
    settings.setValue("SIGNAL HUP", true);
    settings.sync();

    snHup->setEnabled(true);
}


Widget::~Widget()
{
    delete turnOffSplash;

    delete totalTimeSaveTimer;
    delete timerLabelRefresh;
    delete timerZapisu;

    saveTime();
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

    if(newTabNameForm != nullptr)
    {
        delete newTabNameForm;
    }
}


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


void Widget::timerLabelRefreshSlot()
{
    diffTimeString = getDiffTimeString();
    //quint64 userSessiontime_sec ?
    quint64 progamSessionTime_sec = diffTimeString.toInt();
    quint64 dayTime_sec = totalTime + getDiffTimeInt();
    ui->labelDiffTime->setText(secondsToString(progamSessionTime_sec));
    ui->labelTotalTime->setText(secondsToString(dayTime_sec));

    if(dayTime_sec >= 60*60){ //60min*60second
        prepareTurnOffPC();
    }
}


void Widget::prepareTurnOffPC(){
    if(turnOffSplash == nullptr){
        QPixmap pixmap(":/images/splash.svg");
        turnOffSplash = new QSplashScreen(pixmap);
        turnOffSplash->show();
        //QTimer::singleShot(3*60*1000, [&]() {
            turnOffPC();
        //});
    }
}

void Widget::turnOffPC()
{
    qDebug() << "Widget::turnOffPC()";
    QProcess process;
    process.start("shutdown", QStringList() << "-h" << "now");

    if (!process.waitForStarted() || !process.waitForFinished()) {
        qDebug() << "Failed to execute the shutdown command.";
    }
}


QString Widget::getDiffTimeString()
{
    QString diffTimeStringTmp;
    diffTimeStringTmp = QString("%1").arg(getDiffTimeInt());
    return diffTimeStringTmp;
}


qint64 Widget::getDiffTimeInt()
{
    currentTimeString = QDateTime().currentDateTime().toString("hh:mm:ss  dd/MM/yyyy");
    return QDateTime().fromString(startTimeString ,"hh:mm:ss  dd/MM/yyyy").secsTo(QDateTime().fromString(currentTimeString ,"hh:mm:ss  dd/MM/yyyy"));
}


QString Widget::secondsToString(qint64 seconds)
{
    const qint64 DAY = 86400;
    qint64 days = seconds / DAY;
    QTime t = QTime(0,0).addSecs(seconds % DAY);
    return QString("%1 days, %2 hours, %3 minutes, %4 seconds")
            .arg(days).arg(t.hour()).arg(t.minute()).arg(t.second());
}


void Widget::setEditTabProperties(QTextEdit * /*textEdit*/)
{

}


void Widget::showTabContextMenu(QPoint point)
{
    QAction *closeTabAction = new QAction(this);
    closeTabAction->setText("Delete List");
    connect(closeTabAction, SIGNAL(triggered()),this, SLOT(deleteTabRequest()));

    QMenu *tabWidgetContextMenu = new QMenu;
    tabWidgetContextMenu->addAction(closeTabAction);
    tabWidgetContextMenu->exec(ui->tabWidget->mapToGlobal(point));
}


void Widget::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}


void Widget::deleteTabRequest()
{
    qDebug() << "Widget::deleteTab()";

    QMessageBox msgBox;
    msgBox.setWindowTitle("WARNING");
    msgBox.setText("Do you really want delete task list?");
    msgBox.setStandardButtons(QMessageBox::No);
    msgBox.addButton(QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
    {
        this->deleteTab();
    }
    else
    {
        // do something else
    }
}


void Widget::deleteTab()
{
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
    saveNotes();
}


void Widget::on_close()
{
    QApplication::quit();
}


void Widget::onTextChanged()
{
    qDebug() << "On text changed()" ;
    timerZapisu->start(10000); //zapisz zawartosc po 10sekundach od ostatniej zmiany
}


void Widget::timeoutTextChanged(){
    qDebug() << "Save notes timer()";
    timerZapisu->stop();
    saveNotes();
}


void Widget::readAndSetWindowGeometry()
{
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


void Widget::restoreNotes()
{
    QSettings settings(organizationName, applicationName);

    int notesCount = settings.value("notesCount").toInt();

    for(int i = 0; i < notesCount; i++)
    {
        settings.beginGroup(QString::number(i));
        {
            this->insertNewTab(settings.value("noteName").toString(),settings.value("noteContent").toString(), i);
        }
        settings.endGroup();
    }
}

void Widget::saveNotes()
{
    int notesCount = ui->tabWidget->count() - 1;// number of tabs minus "+" tab

    QSettings settings(organizationName, applicationName);
    settings.setValue("notesCount",notesCount);

    for(int i = 0; i < notesCount; i++)
    {
        settings.beginGroup(QString::number(i));
        {
            settings.setValue("noteName", ui->tabWidget->tabText(i));
            settings.setValue("noteContent",(static_cast<QTextEdit *>(ui->tabWidget->widget(i)))->toPlainText());
        }
        settings.endGroup();
    }

    settings.sync();
}


void Widget::restoreTime()
{
    QSettings settings(organizationName, applicationName);

    QString lastDataStarted = settings.value("lastDataStarted").toString();
    settings.setValue("lastDataStarted", getCurrentDate());

    QString currentDate = getCurrentDate();

    if(lastDataStarted != currentDate)
    {
        totalTime = 0;
        settings.setValue("totalTime", 0);
    }
    else
    {
        totalTime = settings.value("totalTime").toInt();
    }

    settings.sync();
}


void Widget::saveTime()
{
    QSettings settings(organizationName, applicationName);
    settings.setValue("totalTime", totalTime + getDiffTimeInt());
    settings.sync();
}


QString Widget::getCurrentDate()
{
    return QDateTime().currentDateTime().toString("dd/MM/yyyy");
}


void Widget::saveWindowGeometry()
{
    QSettings settings(organizationName, applicationName);
    settings.setValue("windowHeight",this->height());
    settings.setValue("windowWidth",this->width());
    settings.setValue("windowXposition",this->pos().x());
    settings.setValue("windowYposition",this->pos().y());
    settings.sync();
}


void Widget::onNewFormNameOKClicked()
{
    QString newTabName = newTabNameForm->getText();
    this->insertNewTab(newTabName,"", ui->tabWidget->currentIndex());
}


void Widget::onNewFormNameCancelClicked()
{

}


void Widget::insertNewTab(QString tabName, QString tabContent, int index)
{
    QTextEdit * textEdit = new QTextEdit();
    textEdit->setText(tabContent);
    ui->tabWidget->insertTab(index,textEdit,tabName);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->currentIndex()-1);

    connect(textEdit,SIGNAL(textChanged()),this,SLOT(onTextChanged()));
}


void Widget::on_tabWidget_tabBarClicked(int index)
{
    if("+" == ui->tabWidget->tabText(index))
    {
        if(newTabNameForm == nullptr)
        {
            newTabNameForm = new NewTabNameForm();
            connect(newTabNameForm,SIGNAL(onOKClicked()),this,SLOT(onNewFormNameOKClicked()));
            connect(newTabNameForm,SIGNAL(onCancelClicked()),this,SLOT(onNewFormNameCancelClicked()));
        }
        newTabNameForm->show();
    }
}
