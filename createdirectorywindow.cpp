#include "createdirectorywindow.h"
#include "ui_createdirectorywindow.h"

CreateDirectoryWindow::CreateDirectoryWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDirectoryWindow)
{
    ui->setupUi(this);
}

CreateDirectoryWindow::~CreateDirectoryWindow()
{
    delete ui;
}

void CreateDirectoryWindow::on_pushButton_clicked()
{
    emit editingFinished(ui->lineEdit->text().toStdString());
    this->close();
}
