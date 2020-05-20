#include "file.h"

#include <QDesktopServices>
#include <QMouseEvent>

File::File( std::string name, struct stat info, QWidget *parent) : Object(name, info, parent)
{
    QPixmap pix ("FileIcon.png");
    this->setScaledContents(true);
    this->setPixmap(pix);
    this->setFixedSize(50, 50);
}

File::File(const File &other) : Object(other)
{

}

File::~File()
{

}

void File::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit execute(this->getName());
    }
}
