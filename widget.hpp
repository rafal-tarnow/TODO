#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include <QTimer>
#include <QTextEdit>
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

private slots:
    void on_close();
    void onTextChanged();
    void timeoutTextChanged();
    void on_tabWidget_tabBarClicked(int index);
    void onNewFormNameOKClicked();
    void onNewFormNameCancelClicked();


private:
    void insertNewTab(QString tabName, QString tabContent, int index);
    void setEditTabProperties(QTextEdit * editText);
    void readAndSetWindowGeometry();
    void saveWindowGeometry();
    void restoreNotes();
    void saveNotes();
    void closeEvent(QCloseEvent *event);
    void createMinimalizeToTry();




    Ui::Widget *ui;

   const QString organizationName;
   const QString applicationName;
   const QString notesContetKey;

   NewTabNameForm * newTabNameForm = nullptr;

   QTimer * timerZapisu;

    QSystemTrayIcon *icon;
    QMenu *menu;
    QAction *hide_window;
    QAction *quitAction;
    QAction *restore;
};

#endif // WIDGET_HPP
