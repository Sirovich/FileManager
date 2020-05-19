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
    if (this->currentDirectory.size() == 1)
    {
        return;
    }

    for(int i = currentDirectory.size()-2; i >= 0; i--)
    {
        if (currentDirectory[i] == '/')
        {
            currentDirectory = currentDirectory.substr(0, i+1);
            break;
        }
    }
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

void Manager::deleteFiles(std::string name)
{
    struct stat info;
    stat((currentDirectory+name).c_str(), &info);
    if (getType(info) == 2)
    {
        deleteDirectory(currentDirectory+name+"/");
    }

    if(std::remove((currentDirectory+name).c_str()) != 0)
    {
        qDebug() << "Error deleting file";
        qDebug() << strerror(errno);
    }

    emit changeUi();
}

void Manager::selecting(std::string name, short keyPressed)
{
    if (keyPressed == 0 || keyPressed == 1)
    {
        for(Directory* directory: directories)
        {
            if (directory->getName() == name)
            {
                directory->setSelected(true);
            }
            else if (directory->isSelected())
            {
                if (keyPressed == 0)
                {
                    directory->setSelected(false);
                }
            }
        }

        for(File* file: files)
        {
            if (file->getName() == name)
            {
                file->setSelected(true);
            }
            else if (file->isSelected())
            {
                if (keyPressed == 0)
                {
                    file->setSelected(false);
                }
            }
        }
    }
    else if (keyPressed == 2)
    {
        bool isFind = false;
        int firstSelected = -1;
        std::vector<Object*> objects = std::vector<Object*>();
        for (Directory* directory: directories)
        {
            objects.push_back(directory);
        }
        for (File* file: files)
        {
            objects.push_back(file);
        }

        for(int i = 0; i < objects.size(); i++)
        {
            if (objects[i]->getName() == name)
            {
                isFind = true;
                objects[i]->setSelected(true);
                if (firstSelected == -1)
                {
                    firstSelected = i;
                    continue;
                }
                else
                {
                    break;
                }
            }

            if (objects[i]->isSelected())
            {
                if (isFind && firstSelected != -1)
                {
                    break;
                }
                else if (firstSelected == -1)
                {
                    firstSelected = i;
                }
            }

            if (firstSelected != -1)
            {
                objects[i]->setSelected(true);
            }
        }
    }
}

void Manager::insertFiles()
{
    for(Object* file: copied)
    {
         std::string fileName;
         for(int i = file->getName().size() - 1; i > 0; i--)
         {
             if (file->getName()[i] == '/')
             {
                 fileName = file->getName().substr(i + 1, std::string::npos);
                 qDebug() << fileName.c_str();
                 break;
             }
         }

         int fileType = getType(file->getInfo());
         if (fileType == 1)
         {
             insertFile(file->getName(), currentDirectory + fileName, file->getInfo().st_size);
         }
         else if (fileType == 2)
         {
             insertDirectory(file->getName(), currentDirectory);
         }

         emit changeUi();
    }
}

void Manager::insertFile(std::string source, std::string destination, long size)
{
    int input, output;

      if( (input=open(source.c_str(), O_RDONLY)) == -1 )
      {
          qDebug() << "cannot open source file";
      }


      if( (output=creat(destination.c_str(), 0644)) == -1 )
      {
          close(input);
          qDebug() << "cannot open destination file";
          qDebug() << strerror(errno);
      }

      off_t bytesCopied = 0;
      int result = sendfile(output, input, &bytesCopied, size);
      if (result == -1)
      {
          qDebug() << "not copied";
      }

      if( close(input) == -1 || close(output) == -1 )
      {
          qDebug() << "close error";
          qDebug() << strerror(errno);
      }
}

void Manager::insertDirectory(std::string source, std::string destination)
{
    DIR *dir_ptr = NULL;
    struct dirent *direntp;
    source += "/";

    if( (dir_ptr = opendir(source.c_str())) == NULL )
    {
        qDebug() << source.c_str();
    }
    else
    {
        while((direntp = readdir(dir_ptr)) != NULL)
        {
            struct stat fileinfo;
            std::string fileName = direntp->d_name;
            stat((source+fileName).c_str(), &fileinfo);
            if (fileName[0] != '.')
            {
                if(getType(fileinfo) == 1)
                {
                    insertFile(source + "/" + direntp->d_name, destination + direntp->d_name, fileinfo.st_size);
                }
                else if (getType(fileinfo) == 2)
                {
                    mkdir((destination + direntp->d_name + "/").c_str(), 0777);
                    insertDirectory(source + direntp->d_name, destination + direntp->d_name + "/");
                }
            }
      }
      closedir(dir_ptr);
   }
}

int Manager::getType(struct stat info)
{
    if ((info.st_mode & S_IFMT) == S_IFREG)
    {
        return 1;
    }

    if ((info.st_mode & S_IFMT) == S_IFDIR)
    {
        return 2;
    }

    return -1;
}

void Manager::deleteDirectory(std::string path)
{
    DIR *dir_ptr = NULL;
    struct dirent *direntp;

    if( (dir_ptr = opendir(path.c_str())) == NULL )
    {

    }
    else
    {
        while((direntp = readdir(dir_ptr)) != NULL)
        {
            struct stat fileinfo;
            std::string fileName = direntp->d_name;
            stat((path+fileName).c_str(), &fileinfo);
            if (fileName[0] != '.')
            {
                if(getType(fileinfo) == 2)
                {
                    deleteDirectory(path + fileName + "/");
                }

                if(std::remove((path + fileName).c_str()) != 0)
                {
                    qDebug() << "Error deleting file";
                    qDebug() << strerror(errno);
                }
            }
      }
      closedir(dir_ptr);
   }
}

void Manager::copySelected()
{
    for(Directory* directory: directories)
    {
        if (directory->isSelected())
        {
            Object* fileInfo = new Object(currentDirectory+directory->getName(), directory->getInfo());
            copied.push_back(fileInfo);
        }
    }

    for(File* file: files)
    {
        if (file->isSelected())
        {
            Object* fileInfo = new Object(currentDirectory+file->getName(), file->getInfo());
            copied.push_back(fileInfo);
        }
    }
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
        connect(directory, &Directory::selecting, this, &Manager::selecting);
        connect(directory, &Directory::deleteSignal, this, &Manager::deleteFiles);
        connect(directory, &Directory::renameSignal, this, &Manager::renameFile);
        connect(directory, &Directory::changed, this, &Manager::directoryChanged);
    }
}

void Manager::connectFiles()
{
    for(File* file: this->files)
    {
        connect(file, &File::selecting, this, &Manager::selecting);
        connect(file, &File::deleteSignal, this, &Manager::deleteFiles);
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
