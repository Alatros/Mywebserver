#include"Connection.h"
#include"Eventloop.h"
#include"Socket.h"
#include"Channel.h"
#include"Buffer.h"
#include "util.h"
#include <cstring>
#include<unistd.h>

#define READ_BUF_SIZE 1024

Connection::Connection(Eventloop* _loop,Socket* _socket):loop(_loop),socket(_socket){
    channel = new Channel(loop,socket->getFd());
    std::function<void()> f = std::bind(&Connection::echo, this, socket->getFd());
    channel->setCallback(f);
    channel->enableReading();
    readBuffer = new Buffer();
}

Connection::~Connection(){
    delete socket;
    delete channel;
}

void Connection::echo(int fd){
    char buf[READ_BUF_SIZE];
    while(true){
        memset(buf, 0, READ_BUF_SIZE);
        ssize_t read_size = read(fd, buf, READ_BUF_SIZE);
        if(read_size>0){
            readBuffer->append(buf, read_size);
        }
        else if(read_size == 0){//客户端关闭
            printf("client %d closed\n", fd);
            removeConnectionCallback(socket);
            break;
        }
        else if(read_size == -1 && ((errno == EAGAIN)||(errno == EWOULDBLOCK))){//缓冲区无数据
            printf("finish reading from client %d once\n", fd);
            printf("client %d send: %s\n", fd, readBuffer->getBuffer());
            errorif(write(fd, readBuffer->getBuffer(), readBuffer->size())==-1, "write error");
            readBuffer->clear();
            break;
        }
        else if(read_size==-1 && (errno == EINTR)){//被信号中断
            printf("client %d interrupted by signal\n", fd);
            continue;
        }
    }
}

void Connection::setRemoveConnectionCallback(std::function<void(Socket*)> f){
    this->removeConnectionCallback = std::move(f);
}