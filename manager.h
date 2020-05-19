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
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <createdirectorywindow.h>
class Manager : public QObject
{
    Q_OBJECT
private:
    std::string currentDirectory;
    std::vector<File*> files;
    std::vector<Directory*> directories;
    std::vector<Object*> copied;
public slots:
    void directoryChanged(std::string directoryName);
    void turnBack();
    void executeFile(std::string fileName);
    void renameFile(std::string oldName, std::string newName);
    void deleteFile(std::string name);
    void selecting(std::string name, short keyPressed);
    void insertFiles();
    void copySelected();
public:
    Manager(QObject* parent = nullptr);
    Manager(const Manager& other);
    Manager& operator=(const Manager& other);
    virtual ~Manager();
    void getAllObjects();
    std::string getCurrentDirectory();
    std::vector<File*> getFiles();
    std::vector<Directory*> getDirectories();
    void sortFilesByType(std::vector<Object> systemFiles);
    void connectDirectories();
    void connectFiles();
    void createDirectory();
    void createFolder(std::string name);
signals:
    void changeUi();
    void displayError(std::string errorMessage);
};

#endif // MANAGER_H
