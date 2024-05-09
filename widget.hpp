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
#include <QTime>
#include <qqml.h>
#include "newtabnameform.hpp"
#include "settingsdialog.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    Q_PROPERTY(bool limitTotalTimeFlag READ limitTotalTimeFlag WRITE setLimitTotalTimeFlag NOTIFY limitTotalTimeFlagChanged FINAL)
    Q_PROPERTY(bool limitStartTimeFlag READ limitStartTimeFlag WRITE setLimitStartTimeFlag NOTIFY limitStartTimeFlagChanged FINAL)
    Q_PROPERTY(QString startTimeLimit READ startTimeLimit WRITE setStartTimeLimit NOTIFY startTimeLimitChanged FINAL)
    Q_PROPERTY(QString totalTimeLimit READ totalTimeLimit WRITE setTotalTimeLimit NOTIFY totalTimeLimitChanged FINAL)
    Q_INVOKABLE void showSettings();

    bool limitTotalTimeFlag();
    void setLimitTotalTimeFlag(bool limit);

    bool limitStartTimeFlag();
    void setLimitStartTimeFlag(bool limit);

    QString startTimeLimit();
    void setStartTimeLimit(QString startTime);

    QString totalTimeLimit();
    void setTotalTimeLimit(QString startTime);

// Unix signal handlers.
    static void hupSignalHandler(int unused);
    static void termSignalHandler(int unused);

signals:
    void limitTotalTimeFlagChanged();
    void limitStartTimeFlagChanged();
    void startTimeLimitChanged();
    void totalTimeLimitChanged();

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

    QString getCurrentDate();

private slots:
    void update();
    void saveTime();
    void saveLimitTotalTimeFlag();
    void saveLimitStartTimeFlag();
    void saveTotalTimeLimit();
    void saveStartTimeLimit();

    void readLimitTotalTimeFlag();
    void readLimitStartTimeFlag();
    void readTotalTimeLimit();
    void readStartTimeLimit();

private:
    Ui::Widget *ui;

    QDateTime mAppStart;

    QTime mPreviousSessionTime;
    QTime mThisSessionTime;
    QTime mTotalSessionTime;

    QTime mStartTimeLimit;
    QTime mTotalTimeLimit;
    bool mLimitStartTimeFlag = true;
    bool mLimitTotalTimeFlag = true;

    const QString organizationName;
    const QString applicationName;
    const QString notesContetKey;

    QQmlEngine * qmlEngine = nullptr;

    NewTabNameForm * newTabNameForm = nullptr;
    SettingsDialog * settingsDialog = nullptr;

    QTimer * timerZapisu;
    QTimer * totalTimeSaveTimer;
    QTimer * timerLabelRefresh;

    QSplashScreen *turnOffSplash = nullptr;

    QSystemTrayIcon *icon;
    QMenu *menu;
    QAction *hide_window;
    QAction *quitAction;
    QAction *restore;

    QString secondsToString(qint64 seconds);
    void turnOffPC();
    void prepareTurnOffPC();
    bool timePassed(const QTime &timeToCompare);
};

#endif // WIDGET_HPP
