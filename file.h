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

class File : public QLabel, public Object
{
    Q_OBJECT
public:
    explicit File(std::string name, struct stat info, QWidget *parent = nullptr);
    File(const File &other);
    File& operator=(const File &other);
    virtual ~File();
    void setSelected(bool state) override;
signals:
    void execute(std::string name);
    void deleteSignal(std::string name);
    void renameSignal(std::string oldName, std::string newName);
    void selecting(std::string name, short key);
private slots:
    void rename(QString name);
    void remove();
    void displayRenameWindow();
    void displayFileInfoWindow();
protected:
    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void contextMenuEvent(QContextMenuEvent *ev) override;
};

#endif // FILE_H
