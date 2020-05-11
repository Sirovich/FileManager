#ifndef FILE_H
#define FILE_H
#include"object.h"
#include <QPushButton>
#include <QLabel>

class File : public QLabel, public Object
{

public:
   explicit File(std::string name, struct stat info, QWidget *parent = nullptr);
    File(const File &other);
    File& operator=(const File &other);
protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
};

#endif // FILE_H
