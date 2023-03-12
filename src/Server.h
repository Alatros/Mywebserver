#pragma once

#include<map>
#include<vector>
class ThreadPool;
class Eventloop;
class Socket;
class Acceptor;
class Connection;

class Server{
    private:
        Eventloop* mainReactor;
        Acceptor* acceptor;
        std::map<int,Connection*> connectMap;
        std::vector<Eventloop*> subReactors;
        ThreadPool *threadPool;
    public:
        Server(Eventloop*);
        ~Server();
        
        void handleReadEvent(int);
        void newConnection(Socket*);
        void removeConnection(Socket*);
};