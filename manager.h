#ifndef MANAGER_H
#define MANAGER_H
#include "directory.h"
#include "file.h"
#include <string>
#include <vector>
#include <dirent.h>
class Manager
{
private:
    std::string currentDirectory;
    std::vector<File> files;
    std::vector<Directory> directories;
    void getAllObjects();
public:
    Manager();
    ~Manager();
    std::string getCurrentDirectory();
    std::vector<File> getFiles();
    std::vector<Directory> getDirectories();
    void moveToDirectory(std::string path);
    void executeFile(std::string path);
};

#endif // MANAGER_H
