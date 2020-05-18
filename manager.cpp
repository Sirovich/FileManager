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
       if (entryName == ".." || entryName == "." || entryName[0] == '.')
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
    connectFiles();
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

void Manager::executeFile(std::string path)
{
    std::string filePath ="xdg-open " + this->currentDirectory + "'" + path + "'";

    int retValue = system(filePath.c_str());
    if (retValue == -1 || WEXITSTATUS(retValue) != 0)
    {
        emit displayError("Cannot open file");
    }
}

void Manager::renameFile(std::string oldName, std::string newName)
{
    if(std::rename((currentDirectory+oldName).c_str(), (currentDirectory+newName).c_str()) != 0)
    {
         qDebug() << "Error rename file";
    }

    emit changeUi();
}

void Manager::deleteFile(std::string name)
{
    if(unlink((currentDirectory+name).c_str()) != 0)
    {
        qDebug() << "Error deleting file";
    }
    emit changeUi();
}

void Manager::sortFilesByType(std::vector<Object> systemFiles)
{
    for(int i = 0; i < systemFiles.size(); i++)
    {
        if ((systemFiles[i].getInfo().st_mode & S_IFMT) == S_IFREG)
        {
            File* file = new File(systemFiles[i].getName(), systemFiles[i].getInfo());
            this->files.push_back(file);
        }
        else if ((systemFiles[i].getInfo().st_mode & S_IFMT) == S_IFDIR)
        {
            Directory* directory = new Directory (systemFiles[i].getName(), systemFiles[i].getInfo());
            this->directories.push_back(directory);
        }
    }
}

void Manager::connectDirectories()
{
    for(Directory* directory: this->directories)
    {
        connect(directory, &Directory::deleteSignal, this, &Manager::deleteFile);
        connect(directory, &Directory::renameSignal, this, &Manager::renameFile);
        connect(directory, &Directory::changed, this, &Manager::directoryChanged);
    }
}

void Manager::connectFiles()
{
    for(File* file: this->files)
    {
        connect(file, &File::deleteSignal, this, &Manager::deleteFile);
        connect(file, &File::renameSignal, this, &Manager::renameFile);
        connect(file, &File::execute, this, &Manager::executeFile);
    }
}

void Manager::createDirectory()
{
    CreateDirectoryWindow* window = new CreateDirectoryWindow();
    connect(window, &CreateDirectoryWindow::editingFinished, this, &Manager::createFolder);
    window->show();
}

void Manager::createFolder(std::string name)
{
    mkdir((currentDirectory+name).c_str(), 0777);
    emit changeUi();
}
