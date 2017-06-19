#include "newtabnameform.hpp"
#include "ui_newtabnameform.h"

NewTabNameForm::NewTabNameForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewTabNameForm)
{
    ui->setupUi(this);
}

QString NewTabNameForm::getText(){
    return ui->lineEdit->text();
}

NewTabNameForm::~NewTabNameForm()
{
    delete ui;
} 

void NewTabNameForm::on_pushButton_OK_clicked()
{
    emit onOKClicked();
    this->close();
}

void NewTabNameForm::on_pushButton_Cancel_clicked()
{
    emit onCancelClicked();
    this->close();
}
