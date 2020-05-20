#include "object.h"

Object::Object(std::string fileName, struct stat fileInfo, QWidget* parent)
{
    this->fileName = fileName;
    this->fileInfo = fileInfo;
    this->selected = false;
}

Object::Object(const Object &other)
{
    this->fileInfo = other.fileInfo;
    this->fileName = other.fileName;
    this->selected = other.selected;
}

Object::~Object()
{

}

struct stat Object::getInfo()
{
    return this->fileInfo;
}

std::string Object::getName()
{
    return this->fileName;
}

void Object::setName(std::string fileName)
{
    this->fileName = fileName;
}

bool Object::isSelected()
{
    return selected;
}

void Object::setSelected(bool state)
{
    selected = state;
    if (state == false)
    {
        this->setStyleSheet("background-color: none;");
    }
    else
    {
        this->setStyleSheet("background-color: yellow;");
    }
}

void Object::displayRenameWindow()
{
    Rename* renameWindow = new Rename();
    connect(renameWindow, &Rename::editingFinished, this, &Object::rename);
    renameWindow->setText(getName());
    renameWindow->show();
}

void Object::displayFileInfoWindow()
{
    FileInfoWindow* fileInfoWindow = new FileInfoWindow(getName(), getInfo());
    fileInfoWindow->show();
}

void Object::rename(QString name)
{
    std::string oldName = getName();
    this->setName(name.toStdString());
    emit renameSignal(oldName, name.toStdString());
}

void Object::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton || ev->button() == Qt::RightButton)
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

void Object::contextMenuEvent(QContextMenuEvent *ev)
{
    QMenu* menu = new QMenu();
    QAction* renameAction = new QAction("Rename", this);
    QAction* removeAction = new QAction("Delete", this);
    QAction* getInfoAction = new QAction("Information", this);
    connect(renameAction, &QAction::triggered, this, &Object::displayRenameWindow);
    connect(removeAction, &QAction::triggered, this, &Object::remove);
    connect(getInfoAction, &QAction::triggered, this, &Object::displayFileInfoWindow);
    menu->addAction(renameAction);
    menu->addAction(removeAction);
    menu->addAction(getInfoAction);
    menu->exec(QCursor::pos());
}


void Object::remove()
{
    setSelected(true);
    emit deleteSignal();
}

