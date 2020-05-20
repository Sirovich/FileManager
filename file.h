#ifndef FILE_H
#define FILE_H
#include"object.h"
#include <QPushButton>
#include <QLabel>
#include "rename.h"
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <unistd.h>
#include <stdio.h>
#include "fileinfowindow.h"

class File : public Object
{
    Q_OBJECT
public:
    File(std::string name, struct stat info, QWidget *parent = nullptr);
    File(const File &other);
    virtual ~File();
signals:
    void execute(std::string name);
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#endif // FILE_H
