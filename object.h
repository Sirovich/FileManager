#ifndef OBJECT_H
#define OBJECT_H
#include<string>
#include<sys/stat.h>
#include<QObject>
class Object
{
private:
    std::string fileName;
    struct stat fileInfo;
    bool selected;
public:
    Object(std::string fileName, struct stat fileInfo);
    Object(const Object& other);
    ~Object();
    struct stat getInfo();
    std::string getName();
    void setName(std::string fileName);
    bool isSelected();
    virtual void setSelected(bool state);
};


#endif // OBJECT_H
