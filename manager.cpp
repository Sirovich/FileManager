#include "manager.h"

void Manager::getAllObjects()
{

}

Manager::Manager()
{

}

Manager::~Manager()
{
    this->files.~vector();
    this->directories.~vector();
    this->currentDirectory.~basic_string();
}

std::string Manager::getCurrentDirectory()
{
    return this->currentDirectory;
}

std::vector<File> Manager::getFiles()
{
    return this->files;
}

std::vector<Directory> Manager::getDirectories()
{
    return this->directories;
}

void Manager::moveToDirectory(std::string path)
{
    this->currentDirectory += "//" + path;
}

void Manager::executeFile(std::string path)
{
    std::string executable = this->currentDirectory + "//" + path;
    std::system(executable.c_str());
}
