#include "Acceptor.h"
#include "Socket.h"
#include "Channel.h"
#include "InetAddress.h"
#include "Eventloop.h"
#include <utility>

Acceptor::Acceptor(Eventloop* loop){
    this->loop = loop;
    this->socket = new Socket();
    InetAddress* addr = new InetAddress("127.0.0.1",8888);
    this->socket->bind(addr);
    this->socket->listen();
    this->socket->setnonblocking();

    this->channel = new Channel(loop, socket->getFd());
    std::function<void()> f = std::bind(&Acceptor::acceptConnection, this);
    this->channel->setCallback(f);
    this->channel->enableReading();
    delete addr;
}

Acceptor::~Acceptor(){
    delete this->socket;
    delete this->channel;
}

void Acceptor::acceptConnection(){
    InetAddress* clt_addr = new InetAddress();
    Socket* clt_socket = new Socket(this->socket->accept(clt_addr));
    printf("new connection from %s:%d\n", clt_addr->getIp(), clt_addr->getPort());
    clt_socket->setnonblocking();
    this->newConnectionCallback(clt_socket);
    delete clt_addr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> f){
    this->newConnectionCallback = std::move(f);
}