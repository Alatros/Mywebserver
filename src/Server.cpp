#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Acceptor.h"
#include "Channel.h"
#include"Connection.h"
#include "ThreadPool.h"
#include"Eventloop.h"

#include <functional>
#include <string.h>
#include <unistd.h>
#include <thread>
#include <iostream>

#define READ_BUF_SIZE 1024

Server::Server(Eventloop* loop):mainReactor(loop){
    acceptor = new Acceptor(mainReactor);
    acceptor->setNewConnectionCallback(std::bind(&Server::newConnection, this, std::placeholders::_1));
    int size = std::thread::hardware_concurrency();
    threadPool = new ThreadPool(size);
    for(int i=0;i<size;i++){
        subReactors.push_back(new Eventloop());
    }
    for(auto &subReactor:subReactors){
        threadPool->addTask(std::bind(&Eventloop::loop, subReactor));
    }
}

Server::~Server(){
    delete acceptor;
    delete threadPool;
}


void Server::newConnection(Socket* serv_sock){
    if(serv_sock->getFd() == -1){
        return;
    }
    int random = serv_sock->getFd()%subReactors.size();
    std::cout<<"socket "<<serv_sock->getFd()<<" is assigned to subReactor "<<random<<std::endl;
    Connection *conn = new Connection(subReactors[random], serv_sock);
    conn->setRemoveConnectionCallback(std::bind(&Server::removeConnection, this, std::placeholders::_1));
    conn->setOnConnectionCallback(this->onConnectionCallback);
    connectMap[serv_sock->getFd()] = conn;
}

void Server::removeConnection(Socket* serv_sock){
    if(serv_sock->getFd() == -1){
        return;
    }
    if(connectMap.find(serv_sock->getFd()) != connectMap.end()){
        delete connectMap[serv_sock->getFd()];
        connectMap.erase(serv_sock->getFd());
    }
}

void Server::setOnConnectionCallback(std::function<void(Connection*)> cb){
    onConnectionCallback = cb;
}