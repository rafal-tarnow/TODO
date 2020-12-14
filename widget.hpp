#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include <QTimer>
#include <QTextEdit>
#include <QPoint>
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
    void saveNotes();
    void closeEvent(QCloseEvent *event);
    void createMinimalizeToTry();

private slots:
    void timerLabelRefreshSlot();

private:
    Ui::Widget *ui;

   const QString organizationName;
   const QString applicationName;
   const QString notesContetKey;

   NewTabNameForm * newTabNameForm = nullptr;

   QTimer * timerZapisu;
   QTimer * timerLabelRefresh;

    QSystemTrayIcon *icon;
    QMenu *menu;
    QAction *hide_window;
    QAction *quitAction;
    QAction *restore;

    QString startTimeString;
    QString currentTimeString;
    QString diffTimeString;
    QString secondsToString(qint64 seconds);
};

#endif // WIDGET_HPP
