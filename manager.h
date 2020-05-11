#ifndef MANAGER_H
#define MANAGER_H
#include "directory.h"
#include "file.h"
#include <string>
#include <vector>
#include <dirent.h>
#include <qdebug.h>
#include <sys/stat.h>
#include <sys/types.h>
class Manager : public QObject
{
    Q_OBJECT
private:
    std::string currentDirectory;
    std::vector<File*> files;
    std::vector<Directory*> directories;

public slots:
    void directoryChanged(std::string directoryName);
    void turnBack();
public:
    Manager(QObject* parent = nullptr);
    Manager(const Manager& other);
    Manager& operator=(const Manager& other);
    ~Manager();
    void getAllObjects();
    std::string getCurrentDirectory();
    std::vector<File*> getFiles();
    std::vector<Directory*> getDirectories();
    void moveToDirectory(std::string path);
    void executeFile(std::string path);
    void sortFilesByType(std::vector<Object> systemFiles);
    void connectDirectories();
signals:
    void changeUi();
};

#endif // MANAGER_H
