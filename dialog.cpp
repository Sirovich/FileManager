#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setText(std::string text)
{
    ui->label->setText(text.c_str());
}

void Dialog::on_button_clicked()
{
    this->close();
}
