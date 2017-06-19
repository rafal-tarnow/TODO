#ifndef NEWTABNAMEFORM_HPP
#define NEWTABNAMEFORM_HPP

#include <QWidget>
#include <QString>

namespace Ui {
class NewTabNameForm;
}

class NewTabNameForm : public QWidget
{
    Q_OBJECT

public:
    explicit NewTabNameForm(QWidget *parent = 0);
    ~NewTabNameForm();

    QString getText();

signals:
    void onOKClicked();
    void onCancelClicked();

private slots:
    void on_pushButton_OK_clicked();
    void on_pushButton_Cancel_clicked();

private:
    Ui::NewTabNameForm *ui;
    QString text;
};

#endif // NEWTABNAMEFORM_HPP
