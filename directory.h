#ifndef DIRECTORY_H
#define DIRECTORY_H
#include "object.h"
#include <QPushButton>
#include <string>
#include <QDebug>
#include <QLabel>
#include <QMouseEvent>

class Directory : public QLabel, public Object
{
    Q_OBJECT
public:
    Directory(std::string name, struct stat info, QWidget *parent = nullptr);
    Directory(const Directory& other);
    Directory& operator=(const Directory& other);
    virtual ~Directory();
signals:
    void changed(std::string fileName);
protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
};

#endif // DIRECTORY_H
