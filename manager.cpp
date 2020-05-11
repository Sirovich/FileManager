#include "manager.h"

void Manager::getAllObjects()
{

    DIR *dir;
    struct dirent *entry;
    struct stat buff;
    std::vector<Object> systemFiles = std::vector<Object>();
    dir = opendir(this->currentDirectory.c_str());
    if (dir == NULL)
    {
        for (int i = currentDirectory.size() - 2; i >= 0; i--)
        {
            if (currentDirectory[i] == '/')
            {
                currentDirectory.resize(i+1);
            }
        }
        dir = opendir(currentDirectory.c_str());
    }
    this->files.clear();
    this->directories.clear();
    while ( (entry = readdir(dir)) != NULL)
    {
       std::string entryName = std::string(entry->d_name);
       if (entryName == ".." || entryName == ".")
       {
           continue;
       }
       std::string fullFilePath = this->currentDirectory + entryName;
       if (stat(fullFilePath.c_str(), &buff) != -1)
       {
           Object systemFile = Object(entry->d_name, buff);
           systemFiles.push_back(systemFile);
       }
    };
    
    sortFilesByType(systemFiles);
    connectDirectories();
}

void Manager::directoryChanged(std::string directoryName)
{
    this->currentDirectory += directoryName + "/";
    emit changeUi();
}

void Manager::turnBack()
{
    this->currentDirectory += "../";
    emit changeUi();
}

Manager::Manager(QObject* parent) : QObject(parent)
{
    this->currentDirectory = "/";
}

Manager::Manager(const Manager &other)
{

}

Manager &Manager::operator=(const Manager &other)
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

std::vector<File*> Manager::getFiles()
{
    return this->files;
}

std::vector<Directory*> Manager::getDirectories()
{
    return this->directories;
}

void Manager::moveToDirectory(std::string path)
{
    this->currentDirectory += "/" + path;
}

void Manager::executeFile(std::string path)
{
    std::string executable = this->currentDirectory + "/" + path;
    std::system(executable.c_str());
}

void Manager::sortFilesByType(std::vector<Object> systemFiles)
{
    for(Object systemFile: systemFiles)
    {
        if ((systemFile.getInfo().st_mode & S_IFMT) == S_IFREG)
        {
            File* file = new File(systemFile.getName(), systemFile.getInfo());
            this->files.push_back(file);
        }
        else if ((systemFile.getInfo().st_mode & S_IFMT) == S_IFDIR)
        {
            Directory* directory = new Directory (systemFile.getName(), systemFile.getInfo());
            this->directories.push_back(directory);
        }
    }
}

void Manager::connectDirectories()
{
    for(Directory* directory: this->directories)
    {
        connect(directory, &Directory::changed, this, &Manager::directoryChanged);
    }
}
