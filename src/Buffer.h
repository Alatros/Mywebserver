#pragma once

#include<string>
#include <sys/types.h>

class Buffer{
private:
    std::string buffer;
public:
    Buffer();
    ~Buffer();
    void append(const char*,int);
    ssize_t size();
    const char* getBuffer();
    void clear();
    void getline();
};