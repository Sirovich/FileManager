#ifndef DIRECTORY_H
#define DIRECTORY_H
#include "object.h"
#include <QPushButton>
#include <string>
#include <QDebug>
#include <QLabel>
#include <QMouseEvent>
#include <QMenu>
#include <QCursor>
#include "rename.h"
#include "fileinfowindow.h"

class Directory : public Object
{
    Q_OBJECT
public:
    Directory(std::string name, struct stat info, QWidget *parent = nullptr);
    Directory(const Directory& other);
    virtual ~Directory();
signals:
    void changed(std::string name);
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
};

#endif // DIRECTORY_H
