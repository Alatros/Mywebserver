#pragma once

#include<map>

class Eventloop;
class Socket;
class Acceptor;
class Connection;

class Server{
    private:
        Eventloop* loop;
        Acceptor* acceptor;
        std::map<int,Connection*> connectMap;
    public:
        Server(Eventloop*);
        ~Server();
        
        void handleReadEvent(int);
        void newConnection(Socket*);
        void removeConnection(Socket*);
};