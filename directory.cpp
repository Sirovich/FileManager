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

void Directory::mousePressEvent(QMouseEvent *event)
{

}

void Directory::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit changed(this->getName());
    }
}


