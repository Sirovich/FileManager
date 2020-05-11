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

void File::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {

    }
}
