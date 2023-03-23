#include"src/Eventloop.h"
#include"src/Server.h"
#include"src/Connection.h"
#include"src/Buffer.h"
#include"src/Socket.h"
#include<iostream>



int main(){
    Eventloop* loop = new Eventloop();
    Server* server = new Server(loop);
    server->setOnConnectionCallback(
        [](Connection* conn)->void {
            conn->read();
            if(conn->getState() == Connection::State::Closed){
                conn->close();
                return;
            }
            std::cout<<"Message from client "<<conn->getSocket()->getFd()<<": "<<conn->getReadBuffer()->getBuffer()<<std::endl;
            conn->setSendBuffer(conn->getReadBuffer()->getBuffer());
            conn->write();
        } 
    );
    loop->loop();
    delete server;
    delete loop;
    return 0;
}