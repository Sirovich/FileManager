#include "fileinfowindow.h"
#include "ui_fileinfowindow.h"

FileInfoWindow::FileInfoWindow(Object* file, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileInfoWindow)
{
    ui->setupUi(this);
    ui->fileName->setText(file->getName().c_str());
    struct stat fileInfo = file->getInfo();
    ui->fileSize->setText(getFileSizeString(fileInfo.st_size).c_str());
    ui->fileType->setText(getTypeString(fileInfo.st_mode).c_str());
    ui->fileAccessTime->setText(getTimeString(fileInfo.st_atim).c_str());
    ui->fileModifiedTime->setText(getTimeString(fileInfo.st_mtim).c_str());
    ui->fileChangeTime->setText(getTimeString(fileInfo.st_ctim).c_str());
}

FileInfoWindow::~FileInfoWindow()
{
    delete ui;
}

std::string FileInfoWindow::getFileSizeString(__off_t size)
{
    int times = 0;
    double totalSize = getFileSize(size, times);
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << totalSize;
    std::string sizeString = ss.str() + " " + getUnitString(times);
    return sizeString;
}

std::string FileInfoWindow::getUnitString(int times)
{
    switch (times)
    {
        case 0: return "bytes";
        case 1: return "kB";
        case 2: return "MB";
        case 3: return "GB";
        case 4: return "TB";
        case 5: return "PB";
    }

    return "";
}

std::string FileInfoWindow::getTypeString(mode_t mode)
{
    switch (mode & S_IFMT)
    {
        case S_IFREG: return "File";
        case S_IFDIR: return "Directory";
    }

    return "";
}

std::string FileInfoWindow::getTimeString(struct timespec time)
{
    char buff[100];
    strftime(buff, sizeof(buff), "%D %T", gmtime(&time.tv_sec));
    return buff;
}

double FileInfoWindow::getFileSize(__off_t size, int &times)
{
    double result = size;
    while(result >= 1024)
    {
        times++;
        result /= 1024;
    }

    return result;
}
