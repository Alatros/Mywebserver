#pragma once

#include <functional>

class Eventloop;
class Socket;
class Channel;

class Acceptor{
private:
    Eventloop* loop;
    Socket* socket;
    Channel* channel;
    std::function<void(Socket*)> newConnectionCallback;
public:
    Acceptor(Eventloop*);
    ~Acceptor();
    void acceptConnection();
    void setNewConnectionCallback(std::function<void(Socket*)>);
};