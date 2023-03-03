#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Acceptor.h"
#include "Channel.h"
#include"Connection.h"
#include <functional>
#include <string.h>
#include <unistd.h>

#define READ_BUF_SIZE 1024

Server::Server(Eventloop* loop):loop(loop){
    acceptor = new Acceptor(loop);
    acceptor->setNewConnectionCallback(std::bind(&Server::newConnection, this, std::placeholders::_1));
}

Server::~Server(){
    delete acceptor;
}


void Server::newConnection(Socket* serv_sock){
    Connection* conn = new Connection(loop, serv_sock);
    conn->setRemoveConnectionCallback(std::bind(&Server::removeConnection, this, std::placeholders::_1));
    connectMap[serv_sock->getFd()] = conn;
}

void Server::removeConnection(Socket* serv_sock){
    int fd = serv_sock->getFd();
    delete connectMap[fd];
    connectMap.erase(fd);
}
