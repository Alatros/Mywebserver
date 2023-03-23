#pragma once

class InetAddress;
class Socket
{
private:
    int fd;
public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(InetAddress*);
    void listen();
    void setnonblocking();

    bool isNonblocking();

    int accept(InetAddress*);
    void connect(InetAddress*);
    void connect(const char*, int);

    int getFd();
};

