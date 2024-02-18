#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include <QTimer>
#include <QTextEdit>
#include <QPoint>
#include <QSocketNotifier>
#include <QSplashScreen>
#include "newtabnameform.hpp"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();


// Unix signal handlers.
    static void hupSignalHandler(int unused);
    static void termSignalHandler(int unused);

public slots:
   // Qt signal handlers.
   void handleSigHup();
   void handleSigTerm();

 private:
   static int sighupFd[2];
   static int sigtermFd[2];

   QSocketNotifier *snHup;
   QSocketNotifier *snTerm;

protected:

private slots:
    void deleteTabRequest();
    void on_close();
    void onTextChanged();
    void timeoutTextChanged();
    void on_tabWidget_tabBarClicked(int index);
    void onNewFormNameOKClicked();
    void onNewFormNameCancelClicked();
    void showTabContextMenu(QPoint );

private:
    void deleteTab();
    void insertNewTab(QString tabName, QString tabContent, int index);
    void setEditTabProperties(QTextEdit * editText);
    void readAndSetWindowGeometry();
    void saveWindowGeometry();
    void restoreNotes();
    void restoreTime();
    void saveNotes();
    void closeEvent(QCloseEvent *event);
    void createMinimalizeToTry();
    QString getDiffTimeString();
    qint64 getDiffTimeInt();

    QString getCurrentDate();

private slots:
    void timerLabelRefreshSlot();
    void saveTime();

private:
    Ui::Widget *ui;

    const QString organizationName;
    const QString applicationName;
    const QString notesContetKey;

    NewTabNameForm * newTabNameForm = nullptr;

    QTimer * timerZapisu;
    QTimer * totalTimeSaveTimer;
    QTimer * timerLabelRefresh;

    QSplashScreen *turnOffSplash = nullptr;

    QSystemTrayIcon *icon;
    QMenu *menu;
    QAction *hide_window;
    QAction *quitAction;
    QAction *restore;

    QString startTimeString;
    QString currentTimeString;
    QString diffTimeString;
    qint64 totalTime;
    QString secondsToString(qint64 seconds);
    void turnOffPC();
    void prepareTurnOffPC();
    bool timePassed(const QTime &timeToCompare);
};

#endif // WIDGET_HPP
