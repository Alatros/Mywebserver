#pragma once

#include"NetAddress.h"
class Socket
{
private:
    int fd;
public:
    Socket();
    Socket(int fd);
    ~Socket();

    void bind(const NetAddress &addr);
    void listen();
    void set_nonblock();

    int accept(NetAddress &addr);
    int connect(const NetAddress &addr);
    int get_fd() const;
};
