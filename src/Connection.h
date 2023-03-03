#pragma once
#include<functional>

class Eventloop;
class Socket;
class Channel;
class Buffer;
class Connection{
private:
    Eventloop* loop;
    Socket* socket;
    Channel* channel;
    std::function<void(Socket*)> removeConnectionCallback;
    Buffer* readBuffer;
public:
    Connection(Eventloop*,Socket*);
    ~Connection();

    void echo(int);//用来回显客户端发送的数据
    void setRemoveConnectionCallback(std::function<void(Socket*)>);
};