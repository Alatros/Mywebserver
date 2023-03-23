#include"Connection.h"
#include"Eventloop.h"
#include"Socket.h"
#include"Channel.h"
#include"Buffer.h"
#include "util.h"
#include <asm-generic/errno-base.h>
#include <cstring>
#include <sys/types.h>
#include<unistd.h>
#include<cassert>
#include<sys/socket.h>
#include<iostream>
#include<thread>


#define READ_BUF_SIZE 1024

Connection::Connection(Eventloop* loop,Socket* sock):loop(loop),socket(sock){
    if(loop!=nullptr){
        channel = new Channel(loop,socket->getFd());
        channel->enableReading();
        channel->useET();
    }
    readBuffer = new Buffer();
    sendBuffer = new Buffer();
    state = State::Connected;
}

Connection::~Connection(){
    if(loop!=nullptr){
        delete channel;
    }
    delete socket;
    delete readBuffer;
    delete sendBuffer;
}
void Connection::read(){
    errorif(state!=State::Connected, "connection state is disconnected!");
    readBuffer->clear();
    if(this->socket->isNonblocking()){
        readNonBlock();
    }
    else{
        readBlock();
    }
}

void Connection::write(){
    errorif(state!=State::Connected, "connection state is disconnected!");
    if(this->socket->isNonblocking()){
        writeNonBlock();
    }
    else{
        writeBlock();
    }
    sendBuffer->clear();
}

void Connection::readNonBlock(){
    int fd = socket->getFd();
    char buf[READ_BUF_SIZE];
    while(true){//非阻塞io，一次直接读取固定大小数据
        memset(buf, 0, READ_BUF_SIZE);
        int n = ::read(fd, buf, READ_BUF_SIZE);
        if(n>0){
            readBuffer->append(buf, n);
        }
        else if(n==-1 &&errno==EINTR){//正常中断继续读取
            printf("read interrupted by signal, continue to read\n");
            continue;
        }
        else if(n==0){//断开连接
            printf("connection closed by peer\n");
            state = State::Closed;
            break;
        }
        else if(n==-1&& (errno==EAGAIN||errno==EWOULDBLOCK)){//读取完毕
            break;
        }
        else{//读取失败
            printf("read error on client %d\n", fd);
            state = State::Failed;
            break;
        }
    }
}

void Connection::writeNonBlock(){
    int fd = socket->getFd();
    char buf[sendBuffer->size()];
    memcpy(buf,sendBuffer->getBuffer(),sendBuffer->size());
    int data_size = sendBuffer->size();
    int data_left = data_size;
    while(data_left){
        auto n = ::write(fd,buf+data_size-data_left,data_left);
        if(n==-1&&errno==EINTR){
            printf("write interrupted by signal, continue to write\n");
            continue;
        }
        else if(n==-1&&errno==EAGAIN){
            break;
        }
        else if(n==-1){
            printf("write error on client %d\n", fd);
            state = State::Failed;
            break;
        }
        else{
            data_left -= n;
        }
    }
}

/*
block io only used for client
*/

void Connection::readBlock(){
    int fd = socket->getFd();
    char buf[READ_BUF_SIZE];
    unsigned int data_size = 0;
    socklen_t len = sizeof(data_size);
    int ret = getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &data_size, &len);
    ssize_t read_size = ::read(fd, buf, data_size);
    if(read_size>0){
        readBuffer->append(buf, read_size);
    }
    else if(read_size==0){
        printf("connection closed by peer\n");
        state = State::Closed;
    }
    else{
        printf("read error on client %d\n", fd);
        state = State::Failed;
    }
}

void Connection::writeBlock(){
    int fd = socket->getFd();
    ssize_t write_size = ::write(fd,sendBuffer->getBuffer(),sendBuffer->size());
    if(write_size==-1){
        printf("write error on client %d\n", fd);
        state = State::Failed;
    }
}

Connection::State Connection::getState(){
    return state;
}

void Connection::setSendBuffer(const char* buf){
    sendBuffer->append(buf, strlen(buf));
}

Buffer* Connection::getReadBuffer(){
    return readBuffer;
}

void Connection::setReadBuffer(const char* str){
    readBuffer->append(str, strlen(str));
}

Buffer* Connection::getSendBuffer(){
    return sendBuffer;
}

void Connection::setRemoveConnectionCallback(std::function<void (Socket *)> cb){
    removeConnectionCallback = cb;
}

void Connection::setOnConnectionCallback(std::function<void (Connection *)> cb){
    onConnectionCallback = cb;
    channel->setreadCallback([this](){onConnectionCallback(this);});
}

Socket* Connection::getSocket(){
    return socket;
}

void Connection::close(){
    removeConnectionCallback(socket);
}
