#pragma once

#include<map>
#include<vector>
#include<functional>
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

        std::function<void(Connection*)> onConnectionCallback;
    public:
        Server(Eventloop*);
        ~Server();
        
        void handleReadEvent(int);
        void newConnection(Socket*);
        void removeConnection(Socket*);
        void setOnConnectionCallback(std::function<void(Connection*)> cb);
};