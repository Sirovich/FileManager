#ifndef OBJECT_H
#define OBJECT_H
#include<string>
#include<sys/stat.h>

class Object
{
private:
    std::string fileName;
    struct stat fileInfo;
public:
    Object(std::string fileName, struct stat fileInfo);
    Object(const Object& other);
    struct stat getInfo();
    std::string getName();
};

#endif // OBJECT_H
