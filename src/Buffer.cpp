#include"Buffer.h"
#include"cstring"
#include<iostream>

Buffer::Buffer(){
    buffer = "";
}
Buffer::~Buffer(){
    buffer = "";
}

void Buffer::append(const char* data,int len){
    buffer.append(data,len);
}

ssize_t Buffer::size(){
    return buffer.size();
}

const char* Buffer::getBuffer(){
    return buffer.c_str();
}

void Buffer::clear(){
    buffer.clear();
}

void Buffer::getline(){
    this->clear();
    std::getline(std::cin,buffer);
}