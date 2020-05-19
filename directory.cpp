#include "directory.h"

Directory::Directory(std::string name, struct stat info, QWidget *parent) : QLabel(parent), Object(name, info)
{

}

Directory::Directory(const Directory &other) : Object(other)
{

}

Directory &Directory::operator=(const Directory &other)
{

}

Directory::~Directory()
{

}

void Directory::setSelected(bool state)
{
    Object::setSelected(state);
    if (state == false)
    {
        this->setStyleSheet("background-color: none;");
    }
    else
    {
        this->setStyleSheet("background-color: yellow;");
    }
}

void Directory::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit changed(this->getName());
    }
}

void Directory::displayRenameWindow()
{
    Rename* renameWindow = new Rename();
    connect(renameWindow, &Rename::editingFinished, this, &Directory::rename);
    renameWindow->setText(getName());
    renameWindow->show();
}

void Directory::displayFileInfoWindow()
{
    FileInfoWindow* fileInfoWindow = new FileInfoWindow(this);
    fileInfoWindow->show();
}

void Directory::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        if (ev->modifiers().testFlag(Qt::ShiftModifier))
        {
            emit selecting(this->getName(), 2);
        }
        else if (ev->modifiers().testFlag(Qt::ControlModifier))
        {
            emit selecting(this->getName(), 1);
        }
        else
        {
            emit selecting(this->getName(), 0);
        }
    }
}

void Directory::contextMenuEvent(QContextMenuEvent *ev)
{
    QMenu* menu = new QMenu();
    QAction* renameAction = new QAction("Rename", this);
    QAction* removeAction = new QAction("Delete", this);
    QAction* getInfoAction = new QAction("Information", this);
    connect(renameAction, &QAction::triggered, this, &Directory::displayRenameWindow);
    connect(removeAction, &QAction::triggered, this, &Directory::remove);
    connect(getInfoAction, &QAction::triggered, this, &Directory::displayFileInfoWindow);
    menu->addAction(renameAction);
    menu->addAction(removeAction);
    menu->addAction(getInfoAction);
    menu->exec(QCursor::pos());
}

void Directory::rename(QString name)
{
    std::string oldName = getName();
    this->setName(name.toStdString());
    emit renameSignal(oldName, name.toStdString());
}

void Directory::remove()
{
    emit deleteSignal(getName());
}
