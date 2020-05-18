#include "file.h"

#include <QDesktopServices>
#include <QMouseEvent>

File::File( std::string name, struct stat info, QWidget *parent) : QLabel(parent), Object(name, info)
{

}

File::File(const File &other) : Object(other)
{

}

File &File::operator=(const File &other)
{

}

File::~File()
{

}

void File::displayRenameWindow()
{
    Rename* renameWindow = new Rename();
    connect(renameWindow, &Rename::editingFinished, this, &File::rename);
    renameWindow->setText(getName());
    renameWindow->show();
}

void File::displayFileInfoWindow()
{
    FileInfoWindow* fileInfoWindow = new FileInfoWindow(this);
    fileInfoWindow->show();
}

void File::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        QWidget* widget = this->parentWidget();
        this->setStyleSheet("background-color: red;");
        widget->setStyleSheet("border-width: 1px;");
        widget->setStyleSheet("border-color: red;");
        widget->setStyleSheet("border-style: solid;");
        qDebug() << "123";
    }
}

void File::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit execute(this->getName());
    }
}

void File::contextMenuEvent(QContextMenuEvent *ev)
{
    QMenu* menu = new QMenu();
    QAction* renameAction = new QAction("Rename", this);
    QAction* removeAction = new QAction("Delete", this);
    QAction* getInfoAction = new QAction("Information", this);
    connect(renameAction, &QAction::triggered, this, &File::displayRenameWindow);
    connect(removeAction, &QAction::triggered, this, &File::remove);
    connect(getInfoAction, &QAction::triggered, this, &File::displayFileInfoWindow);
    menu->addAction(renameAction);
    menu->addAction(removeAction);
    menu->addAction(getInfoAction);
    menu->exec(QCursor::pos());
}

void File::rename(QString name)
{
    std::string oldName = getName();
    this->setName(name.toStdString());
    emit renameSignal(oldName, name.toStdString());
}

void File::remove()
{
    emit deleteSignal(getName());
}
