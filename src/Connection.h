#pragma once
#include<functional>

class Eventloop;
class Socket;
class Channel;
class Buffer;
class Connection{  
public:  
    enum State{
        Invalid=1,
        Handshaking,
        Connected,
        Closed,
        Failed
    };
    enum BlockState{
        ReadBlock=1,
        WriteBlock,
        
    };
private:
    Eventloop* loop;
    Socket* socket;
    Channel* channel;
    std::function<void(Socket*)> removeConnectionCallback;
    

    State state{Invalid};
    Buffer* sendBuffer;
    Buffer* readBuffer;

    std::function<void(Connection*)> onConnectionCallback;

    void readBlock();
    void writeBlock();
    void readNonBlock();
    void writeNonBlock();
public:



    Connection(Eventloop*,Socket*);
    ~Connection();

    Connection(const Connection&)=delete;
    Connection& operator=(const Connection&)=delete;
    Connection(Connection&&)=delete;

    void read();
    void write();


    void setOnConnectionCallback(std::function<void(Connection*)> cb);
    void setRemoveConnectionCallback(std::function<void(Socket*)>cb);
    State getState();
    void close();
    void setSendBuffer(const char*);
    Buffer *getReadBuffer();
    void setReadBuffer(const char*);
    Buffer *getSendBuffer();

    Socket* getSocket();

};