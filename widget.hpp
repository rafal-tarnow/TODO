#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>

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

private:
    void readAndSetWindowGeometry();
    void saveWindowGeometry();
    void saveNotes();
    void closeEvent(QCloseEvent *event);
    void createMinimalizeToTry();




    Ui::Widget *ui;

   const QString organizationName;
   const QString applicationName;

    QSystemTrayIcon *icon;
    QMenu *menu;
    QAction *hide_window;
    QAction *quitAction;
    QAction *restore;
};

#endif // WIDGET_HPP
