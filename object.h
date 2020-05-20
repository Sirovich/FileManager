#ifndef OBJECT_H
#define OBJECT_H
#include<string>
#include<sys/stat.h>
#include<QLabel>
#include<QContextMenuEvent>
#include<QMenu>
#include<QAction>
#include"rename.h"
#include"fileinfowindow.h"
class Object : public QLabel
{
    Q_OBJECT
private:
    std::string fileName;
    struct stat fileInfo;
    bool selected;
signals:
    void deleteSignal();
    void renameSignal(std::string oldName, std::string newName);
    void selecting(std::string name, short key);
public:
    explicit Object(std::string fileName, struct stat fileInfo, QWidget *parent = nullptr);
    Object(const Object& other);
    virtual ~Object();
    struct stat getInfo();
    std::string getName();
    void setName(std::string fileName);
    bool isSelected();
    void setSelected(bool state);
protected:
    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void contextMenuEvent(QContextMenuEvent *ev) override;
private slots:
    void rename(QString name);
    void remove();
    void displayRenameWindow();
    void displayFileInfoWindow();
};


#endif // OBJECT_H
