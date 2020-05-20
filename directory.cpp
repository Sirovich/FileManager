#include "directory.h"

Directory::Directory(std::string name, struct stat info, QWidget *parent) : Object(name, info, parent)
{
    this->setScaledContents(true);
    QPixmap pix ("DirectoryIcon.png");
    this->setScaledContents(true);
    this->setPixmap(pix);
    this->setFixedSize(50, 50);
}

Directory::Directory(const Directory &other) : Object(other)
{

}

Directory::~Directory()
{

}

void Directory::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit changed(this->getName());
    }
}
