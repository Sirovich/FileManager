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

class Directory : public QLabel, public Object
{
    Q_OBJECT
public:
    Directory(std::string name, struct stat info, QWidget *parent = nullptr);
    Directory(const Directory& other);
    Directory& operator=(const Directory& other);
    virtual ~Directory();
    void setSelected(bool state) override;
private slots:
    void rename(QString name);
    void remove();
    void displayRenameWindow();
    void displayFileInfoWindow();
signals:
    void changed(std::string fileName);
    void renameSignal(std::string oldName, std::string newName);
    void deleteSignal(std::string name);
    void selecting(std::string name, short key);
protected:
    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void contextMenuEvent(QContextMenuEvent *ev) override;
};

#endif // DIRECTORY_H
