#include "object.h"

Object::Object(std::string fileName, struct stat fileInfo)
{
    this->fileName = fileName;
    this->fileInfo = fileInfo;
    this->selected = false;
}

Object::Object(const Object &other)
{
    this->fileInfo = other.fileInfo;
    this->fileName = other.fileName;
    this->selected = other.selected;
}

Object::~Object()
{

}

struct stat Object::getInfo()
{
    return this->fileInfo;
}

std::string Object::getName()
{
    return this->fileName;
}

void Object::setName(std::string fileName)
{
    this->fileName = fileName;
}

bool Object::isSelected()
{
    return selected;
}

void Object::setSelected(bool state)
{
    selected = state;
}

