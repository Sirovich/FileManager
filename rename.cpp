#include "rename.h"
#include "ui_rename.h"

Rename::Rename(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Rename)
{
    ui->setupUi(this);
    ui->lineEdit->setTextMargins(5, 2, 5, 2);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &Rename::textChanged);
}

Rename::~Rename()
{
    delete ui;
}

void Rename::setText(std::string text)
{
    ui->lineEdit->setText(text.c_str());
}

void Rename::textChanged(const QString& text)
{
    if (text.size() == 0)
    {
        ui->pushButton->setEnabled(false);
    }
}

void Rename::on_pushButton_clicked()
{
    emit editingFinished(ui->lineEdit->text());
    this->close();
}
