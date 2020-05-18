#ifndef FILEINFOWINDOW_H
#define FILEINFOWINDOW_H

#include <QDialog>
#include <sys/types.h>
#include "object.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

namespace Ui {
class FileInfoWindow;
}

class FileInfoWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FileInfoWindow(Object* file, QWidget *parent = nullptr);
    ~FileInfoWindow();
private:
    std::string getFileSizeString(__off_t size);
    std::string getUnitString(int times);
    std::string getTypeString(mode_t mode);
    std::string getTimeString(struct timespec time);
    double getFileSize(__off_t size, int& times);
private:
    Ui::FileInfoWindow *ui;
};

#endif // FILEINFOWINDOW_H
